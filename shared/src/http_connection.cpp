#include <cassert>

#include "../include/http_client.h"

#include "../include/http_connection.h"

namespace
{
    static const int ConnectTimeout = 1000; // millisececonds
}

namespace m2
{
    size_t headerCallback(char* data, size_t sizeInItems, size_t bytesInItem, void* connectionPtr)
    {
        auto connection = static_cast<HttpConnection*>(connectionPtr);
        assert(connection);

        const auto size = sizeInItems * bytesInItem;
        connection->header_.assign(data, data + size);
        return size;
    }

    size_t writeCallback(char* data, size_t sizeInItems, size_t bytesInItem, void* connectionPtr)
    {
        auto connection = static_cast<HttpConnection*>(connectionPtr);
        assert(connection);

        const auto size = sizeInItems * bytesInItem;
        connection->writeFunction_(data, size);
        return size;
    }

    int progressCallback(
        void* connectionPtr, double downloadTotal, double downloadNow, double /*uploadedTotal*/, double /*uploadedNow*/)
    {
        auto connection = static_cast<HttpConnection*>(connectionPtr);
        assert(connection);

        if (connection->progress_)
        {
            connection->progress_(downloadNow, downloadTotal);
        }

        return 0;
    }
}

void m2::HttpConnection::perform(
    const HttpRequest& request, DataStream& responseData, CompletionHandler completion, ProgressHandler progress)
{
    auto writeFunction =
        [&responseData](char* data, size_t size)
        {
            responseData.write(data, size);
        };
    perform(request, writeFunction, completion, progress);
}

void m2::HttpConnection::perform(
    const HttpRequest& request, Data& responseData, CompletionHandler completion, ProgressHandler progress)
{
    auto writeFunction =
        [&responseData](char* data, size_t size)
        {
            responseData.insert(responseData.end(), data, data + size);
        };
    perform(request, writeFunction, completion, progress);
}

void m2::HttpConnection::perform(const HttpRequest& request, const Data& requestData, Data& responseData,
    CompletionHandler completion, ProgressHandler progress)
{
    //const char* sectionName = "data";

    assert(curlPost_ == nullptr);

    struct curl_httppost* last = nullptr;

    curl_easy_setopt(curl_, CURLOPT_HTTPPOST, curlPost_);
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, requestData.data());
    perform(request, responseData, completion, progress);
}

void m2::HttpConnection::perform(const HttpRequest& request, WriteFunction writeFunction, CompletionHandler completion, ProgressHandler progress)
{
    completion_ = completion;
    progress_ = progress;

    writeFunction_ = writeFunction;

    header_.clear();

    const auto url = domain_ + request.uri_;
    curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());

    parent_->perform(this, request.timeout_);
}

void m2::HttpConnection::onPerformComplete(CURLcode result)
{
    if (curlPost_)
    {
        curl_formfree(curlPost_);
        curlPost_ = nullptr;
    }

    if (completion_)
    {
        auto response = std::make_unique<HttpResponse>();

        response->code = 0;

        if (result == CURLE_OK)
        {
            curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &response->code);
            response->header.swap(header_);
        }
        else
        {
            header_.clear();
        }
        if (result != CURLE_OK) {
            auto buf = curl_easy_strerror(result);
            response->header.insert(response->header.begin(), buf, buf + strlen(buf));
        }
        //TODO add actual codes

        completion_(result == CURLE_OK ? PerformResult::Success : PerformResult::NetworkError, std::move(response));
    }
}

void m2::HttpConnection::cancel()
{
    parent_->cancel(this);
}

m2::HttpConnection::HttpConnection(const std::string& domain, CURL* curl, HttpClient* parent)
    : domain_(std::move(domain))
    , curl_(curl)
    , parent_(parent)
    , curlPost_(nullptr)
{
    assert(!domain_.empty());
    assert(curl_);
    assert(parent_);

    curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYHOST, 2L);

    curl_easy_setopt(curl_, CURLOPT_NOSIGNAL, 1L);

    auto thisAsVoid = static_cast<void*>(this);

    curl_easy_setopt(curl_, CURLOPT_HEADERDATA, thisAsVoid);
    curl_easy_setopt(curl_, CURLOPT_HEADERFUNCTION, headerCallback);

    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, thisAsVoid);
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, writeCallback);

    curl_easy_setopt(curl_, CURLOPT_NOPROGRESS, false);
    curl_easy_setopt(curl_, CURLOPT_PROGRESSDATA, thisAsVoid);
    curl_easy_setopt(curl_, CURLOPT_PROGRESSFUNCTION, progressCallback);

    curl_easy_setopt(curl_, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl_, CURLOPT_TCP_KEEPIDLE, 5L);
    curl_easy_setopt(curl_, CURLOPT_TCP_KEEPINTVL, 5L);
    curl_easy_setopt(curl_, CURLOPT_NOSIGNAL, 1L);

    curl_easy_setopt(curl_, CURLOPT_ACCEPT_ENCODING, "gzip");

    curl_easy_setopt(curl_, CURLOPT_USERAGENT, "m2");

    curl_easy_setopt(curl_, CURLOPT_TIMEOUT_MS, 0);

    curl_easy_setopt(curl_, CURLOPT_CONNECTTIMEOUT_MS, ConnectTimeout);

    curl_easy_setopt(curl_, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl_, CURLOPT_MAXREDIRS, 10L);
}

m2::HttpConnection::~HttpConnection()
{
    assert(!curl_);
    assert(!parent_);
}

void m2::HttpConnection::close()
{
    curl_ = nullptr;
    parent_ = nullptr;
}
