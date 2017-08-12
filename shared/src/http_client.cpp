#include <cassert>
#include <future>

#include <http_client.h>

namespace
{
    const int MAX_TRANSMISSIONS = 8;
}

void m2::startNewJob(HttpClient* client)
{
    if (client->keepWorking_)
    {
        event_active(client->startTaskEvent_, 0, 0);
    }
}

void m2::finishJob(HttpClient* client, CURL* curl, CURLcode result)
{
    auto it = client->contexts_.find(curl);
    assert(it != client->contexts_.end());

    if (it != client->contexts_.end())
    {
        auto context = it->second.get();
        context->completionCallback_(result);
        client->contexts_.erase(it);
    }
}

void m2::checkMultiInfo(HttpClient* client)
{
    int messagesLeft = 0;
    while (auto message = curl_multi_info_read(client->curlMulti_, &messagesLeft))
    {
        if (message->msg == CURLMSG_DONE)
        {
            const auto curl = message->easy_handle;
            const auto result = message->data.result;

            curl_multi_remove_handle(client->curlMulti_, curl);
            finishJob(client, curl, result);
            startNewJob(client);
        }
    }
}

void m2::addSocket(curl_socket_t socket, CURL* curl, int action, HttpClient* client)
{
    const auto contextIter = client->contexts_.find(curl);
    assert(contextIter != client->contexts_.end());

    const auto context = contextIter->second.get();
    context->update(socket, action);
    curl_multi_assign(client->curlMulti_, socket, context);
}

int m2::socketCallback(CURL* curl, curl_socket_t socket, int what, void* clientPtr, void* contextPtr)
{
    const auto client = static_cast<HttpClient*>(clientPtr);
    const auto context = static_cast<HttpClient::Context*>(contextPtr);

    if (what == CURL_POLL_REMOVE)
    {
        context->freeEvent();
    }
    else if (context == nullptr)
    {
        addSocket(socket, curl, what, client);
    }
    else
    {
        context->update(socket, what);
    }

    return 0;
}

int m2::timerCallback(CURLM* curlMulti, Milliseconds timeout, void* clientPtr)
{
    const auto client = static_cast<HttpClient*>(clientPtr);

    if (timeout == -1)
    {
        evtimer_del(client->timerEvent_);
    }
    else if (timeout == 0)
    {
        curl_multi_socket_action(client->curlMulti_, CURL_SOCKET_TIMEOUT, 0, &client->running_);
    }
    else
    {
        const auto tv = HttpClient::makeTimeval(timeout);
        evtimer_add(client->timerEvent_, &tv);
    }

    return 0;
}

void m2::eventCallback(int socket, short kind, void *contextPtr)
{
    const auto context = static_cast<HttpClient::Context*>(contextPtr);

    const auto tv = HttpClient::makeTimeval(context->timeout_);
    event_del(context->timeoutEvent_);
    event_add(context->timeoutEvent_, &tv);

    int action = 0;
    if (kind & EV_READ)
        action |= CURL_CSELECT_IN;
    if (kind & EV_WRITE)
        action |= CURL_CSELECT_OUT;

    const auto client = context->client_;
    curl_multi_socket_action(client->curlMulti_, socket, action, &client->running_);
    checkMultiInfo(client);
    if (client->running_ <= 0)
    {
        if (evtimer_pending(client->timerEvent_, nullptr))
            evtimer_del(client->timerEvent_);
    }
}

void m2::eventTimerCallback(evutil_socket_t descriptor, short flags, void* clientPtr)
{
    const auto client = static_cast<HttpClient*>(clientPtr);
    curl_multi_socket_action(client->curlMulti_, CURL_SOCKET_TIMEOUT, 0, &client->running_);
    checkMultiInfo(client);
}

void m2::eventTimeoutCallback(evutil_socket_t /*descriptor*/, short /*flags*/, void* contextPtr)
{
    const auto context = static_cast<HttpClient::Context*>(contextPtr);

    curl_multi_remove_handle(context->client_->curlMulti_, context->curl_);

    const auto client = context->client_;
    finishJob(client, context->curl_, CURLE_OPERATION_TIMEDOUT);
    startNewJob(context->client_);
}

void m2::startTaskCallback(evutil_socket_t /*descriptor*/, short /*flags*/, void* clientPtr)
{
    const auto client = static_cast<HttpClient*>(clientPtr);

    using ContextPtr = std::unique_ptr<HttpClient::Context>;
    std::vector<ContextPtr> toProcess;

    {
        std::lock_guard<decltype(client->jobsMutex_)> lock(client->jobsMutex_);

        auto transmissions = client->contexts_.size();

        while (!client->pendingJobs_.empty())
        {
            const auto& job = client->pendingJobs_.front();

            if (transmissions > MAX_TRANSMISSIONS)
                break;

            ++transmissions;

            const auto completionCallback = job.completionCallback_;
            const auto timeout = job.timeout_;
            const auto curl = job.curl_;

            auto context = std::make_unique<HttpClient::Context>(timeout, client, curl, completionCallback);
            toProcess.push_back(std::move(context));

            client->pendingJobs_.pop();
        }
    }

    for (auto&& context : toProcess)
    {
        const auto curl = context->curl_;
        client->contexts_[curl] = std::move(context);

        const auto result = curl_multi_add_handle(client->curlMulti_, curl);
        if (result != CURLM_OK)
        {
            finishJob(client, curl, CURLE_FAILED_INIT);
        }
    }
}

m2::HttpClient::HttpClient()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);

    curlMulti_ = curl_multi_init();
    assert(curlMulti_);

    curl_multi_setopt(curlMulti_, CURLMOPT_SOCKETFUNCTION, socketCallback);
    curl_multi_setopt(curlMulti_, CURLMOPT_SOCKETDATA, this);

    curl_multi_setopt(curlMulti_, CURLMOPT_TIMERFUNCTION, timerCallback);
    curl_multi_setopt(curlMulti_, CURLMOPT_TIMERDATA, this);

    eventBase_ = event_base_new();
    timerEvent_ = evtimer_new(eventBase_, eventTimerCallback, this);
    startTaskEvent_ = event_new(eventBase_, -1, EV_PERSIST, startTaskCallback, this);

    keepWorking_ = true;

    eventLoopThread_ = std::thread([this]()
    {
        while (keepWorking_)
        {
            event_base_dispatch(eventBase_);

            if (keepWorking_)
            {
                std::unique_lock<decltype(eventLoopMutex_)> lock(eventLoopMutex_);
                hasEventToRun_.wait(lock);
            }
        }
    });
}

m2::HttpClient::~HttpClient()
{
    stop();

    curl_global_cleanup();
}

void m2::HttpClient::stop()
{
    keepWorking_ = false;

    event_base_loopbreak(eventBase_);

    hasEventToRun_.notify_one();

    if (eventLoopThread_.joinable())
    {
        eventLoopThread_.join();
    }

    for (auto& it : contexts_)
    {
        curl_multi_remove_handle(curlMulti_, it.first);

        const auto& context = it.second;
        context->completionCallback_(CURLE_ABORTED_BY_CALLBACK);
    }

    contexts_.clear();

    while (!pendingJobs_.empty())
    {
        auto& job = pendingJobs_.front();
        job.completionCallback_(CURLE_ABORTED_BY_CALLBACK);
        pendingJobs_.pop();
    }

    for (auto& connection : connections_)
    {
        connection->close();
        curl_easy_cleanup(connection->curl_);
    }
}

m2::HttpConnectionPtr m2::HttpClient::connect(const std::string& domain)
{
    auto curl = curl_easy_init();
    assert(curl);
    auto connection = std::shared_ptr<HttpConnection>(new HttpConnection(domain, curl, this));
    connections_.insert(connection);
    return connection;
}

void m2::HttpClient::close(HttpConnectionPtr connection)
{
    auto it = connections_.find(connection);
    assert(it != connections_.end());
    if (it != connections_.end())
    {
        connection->close();
        curl_easy_cleanup(connection->curl_);
        connections_.erase(it);
    }
}

void m2::HttpClient::perform(HttpConnection* connection, const std::chrono::milliseconds& timeout)
{
    if (keepWorking_)
    {
        {
            std::lock_guard<decltype(jobsMutex_)> lock(jobsMutex_);

            CompletionCallback callback = std::bind(&HttpConnection::onPerformComplete, connection, std::placeholders::_1);
            pendingJobs_.emplace(timeout, connection->curl_, callback);
        }

        event_active(startTaskEvent_, 0, 0);

        hasEventToRun_.notify_one();
    }
    else
    {
        connection->onPerformComplete(CURLE_FAILED_INIT);
    }
}

void m2::HttpClient::cancel(HttpConnection* connection)
{
}

timeval m2::HttpClient::makeTimeval(const std::chrono::milliseconds& value)
{
    return makeTimeval(value.count());
}

timeval m2::HttpClient::makeTimeval(Milliseconds value)
{
    struct timeval tv;
    tv.tv_sec = value / 1000;
    tv.tv_usec = value - tv.tv_sec * 1000;
    return tv;
}

m2::HttpClient::Context::Context(const std::chrono::milliseconds& timeout, HttpClient* client, CURL* curl, const CompletionCallback& completionCallback)
    : timeout_(timeout)
    , client_(client)
    , curl_(curl)
    , completionCallback_(completionCallback)
    , socket_(0)
    , event_(nullptr)
{
    const auto tv = makeTimeval(timeout_);
    timeoutEvent_ = evtimer_new(client->eventBase_, eventTimeoutCallback, this);
    evtimer_add(timeoutEvent_, &tv);
}

m2::HttpClient::Context::~Context()
{
    freeEvent();
    event_free(timeoutEvent_);
}

void m2::HttpClient::Context::update(curl_socket_t socket, int action)
{
    socket_ = socket;

    if (event_)
        freeEvent();

    auto kind = EV_PERSIST;
    if (action & CURL_POLL_IN)
        kind |= EV_READ;
    if (action & CURL_POLL_OUT)
        kind |= EV_WRITE;

    event_ = event_new(client_->eventBase_, socket_, kind, reinterpret_cast<event_callback_fn>(eventCallback), this);
    event_add(event_, NULL);
}

void m2::HttpClient::Context::freeEvent()
{
    if (event_)
    {
        event_free(event_);
        event_ = nullptr;
    }
}

m2::HttpClient::Job::Job(const std::chrono::milliseconds& timeout, CURL* curl, const CompletionCallback& completionCallback)
    : timeout_(timeout)
    , curl_(curl)
    , completionCallback_(completionCallback)
{
}

#include <iostream>
#include <fstream>

std::future<void> download(m2::HttpConnectionPtr connection, const std::string& uri)
{
    auto promise = std::make_shared<std::promise<void>>();
    auto buffer = std::make_shared<std::vector<char>>();

    connection->perform({ uri, std::chrono::milliseconds(1000) }, *buffer,
        [connection, promise, buffer](m2::PerformResult result, m2::HttpResponsePtr&& response)
        {
            std::cout << static_cast<int>(result) << std::endl;
            if (response)
            {
                std::cout << buffer->size() << std::endl;
                std::cout << response->code << std::endl;
            }
            promise->set_value();
        },
        [](uint64_t receivedBytes, uint64_t totalBytes)
        {
            if (receivedBytes == 0)
                return;
            std::cout << receivedBytes << '/' << totalBytes << std::endl;
        });

    return promise->get_future();
}

//int main()
//{
//    auto client = std::make_shared<m2::HttpClient>();

//    auto c1 = client->connect("http://kirillovka.ks.ua");

//    std::thread t1([c1]()
//    {
//        download(c1, "/wp-content/uploads/2012/06/azovsea-rest.jpg").wait();
//        download(c1, "/wp-content/uploads/2016/03/kirillovka-howto05.jpg").wait();
//        download(c1, "/wp-content/uploads/2015/06/biryuchii-2.jpg").wait();
//        download(c1, "/wp-content/uploads/2015/06/biryuchii-1.jpg").wait();
//    });

//    auto c2 = client->connect("https://curl.haxx.se");

//    std::thread t2([c2]()
//    {
//        download(c2, "/libcurl/c/libcurl-errors.html#CURLMOK").wait();
//        download(c2, "/libcurl/c/CURLOPT_HEADERFUNCTION.html").wait();
//    });

//    auto c3 = client->connect("http://orig10.deviantart.net");

//    std::thread t3([c3]()
//    {
//        download(c3, "/51b6/f/2008/189/4/e/mountain_by_cablekevin.jpg").wait();
//    });

//    auto c4 = client->connect("http://images.summitpost.org");

//    std::thread t4([c4]()
//    {
//        download(c4, "/original/973786.jpg").wait();
//    });

//    auto c5 = client->connect("https://www.perc.org");

//    std::thread t5([c5]()
//    {
//        download(c5, "/sites/default/files/field/image/Forest_54882631.jpg").wait();
//    });

//    t1.join();
//    t2.join();
//    t3.join();
//    t4.join();
//    t5.join();

//    client->close(c1);
//    client->close(c2);
//    client->close(c3);
//    client->close(c4);
//    client->close(c5);

//    client->stop();

//    return 0;
//}
