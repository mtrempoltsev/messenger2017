#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <queue>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>

#include <curl/curl.h>

#include <event2/event.h>

#include <http_connection.h>

/*
    You can use one HttpConnection for multiple requests and you must do this.
    Create a new HttpConnection only for the new domain or if you want to work
    with the server in parallel. When the HttpConnection is deleted
    the connection will be closed.

    https://server.com/user=1234
    |---- domain ----| |- uri -|
*/

namespace m2
{
    class HttpClient;

    using Milliseconds = long;

    void startNewJob(HttpClient* client);
    void finishJob(HttpClient* client, CURL* curl, CURLcode result);
    void checkMultiInfo(HttpClient* client);
    void addSocket(curl_socket_t socket, CURL* curl, int action, HttpClient* client);
    int socketCallback(CURL* curl, curl_socket_t socket, int what, void* clientPtr, void* contextPtr);
    int timerCallback(CURLM* curlMulti, Milliseconds timeout, void* clientPtr);
    void eventCallback(int socket, short kind, void *contextPtr);
    void eventTimerCallback(evutil_socket_t descriptor, short flags, void* clientPtr);
    void eventTimeoutCallback(evutil_socket_t descriptor, short flags, void* clientPtr);
    void startTaskCallback(evutil_socket_t descriptor, short flags, void* clientPtr);

    class HttpClient final
    {
        friend void startNewJob(HttpClient* client);
        friend void finishJob(HttpClient* client, CURL* curl, CURLcode result);
        friend void checkMultiInfo(HttpClient* client);
        friend void addSocket(curl_socket_t socket, CURL* curl, int action, HttpClient* client);
        friend int socketCallback(CURL* curl, curl_socket_t socket, int what, void* clientPtr, void* contextPtr);
        friend int timerCallback(CURLM* curlMulti, Milliseconds timeout, void* clientPtr);
        friend void eventCallback(int socket, short kind, void *contextPtr);
        friend void eventTimerCallback(evutil_socket_t descriptor, short flags, void* clientPtr);
        friend void eventTimeoutCallback(evutil_socket_t descriptor, short flags, void* contextPtr);
        friend void startTaskCallback(evutil_socket_t descriptor, short flags, void* clientPtr);

        friend class HttpConnection;
    public:
        HttpClient();
        ~HttpClient();

        void stop();

        HttpConnectionPtr connect(const std::string& domain);
        void close(HttpConnectionPtr connection);

    private:
        void perform(HttpConnection* connection, const std::chrono::milliseconds& timeout);
        void cancel(HttpConnection* connection);

    private:
        static timeval makeTimeval(const std::chrono::milliseconds& value);
        static timeval makeTimeval(Milliseconds value);

        std::unordered_set<HttpConnectionPtr> connections_;

        using CompletionCallback = std::function<void (CURLcode result)>;

        struct Context
        {
            Context(const std::chrono::milliseconds& timeout, HttpClient* client, CURL* curl, const CompletionCallback& completionCallback);
            ~Context();

            void update(curl_socket_t socket, int action);

            void freeEvent();

            std::chrono::milliseconds timeout_;
            event* timeoutEvent_;

            HttpClient* client_;

            CURL* curl_;

            CompletionCallback completionCallback_;

            curl_socket_t socket_;

            event* event_;
        };

        CURLM* curlMulti_;
        int running_;

        event_base* eventBase_;
        event* timerEvent_;
        event* startTaskEvent_;

        std::thread eventLoopThread_;
        std::mutex eventLoopMutex_;
        std::condition_variable hasEventToRun_;

        std::unordered_map<CURL*, std::unique_ptr<Context>> contexts_;

        struct Job
        {
            Job(const std::chrono::milliseconds& timeout, CURL* curl, const CompletionCallback& completionCallback);

            std::chrono::milliseconds timeout_;
            CURL* curl_;
            CompletionCallback completionCallback_;
        };

        std::queue<Job> pendingJobs_;
        std::mutex jobsMutex_;

        std::atomic<bool> keepWorking_;
    };
}
