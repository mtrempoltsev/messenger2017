#pragma once

#include <chrono>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

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
    using Data = std::vector<char>;
    using DataStream = std::ostream;

    class HttpResponse final
    {
    public:
        long code() const;
        const Data& header() const;
    };

    using HttpResponsePtr = std::unique_ptr<HttpResponse>;

    struct HttpRequest final
    {
        std::string uri;
        std::chrono::milliseconds timeout;
    };

    class HttpConnection final
    {
    public:
        enum class Result
        {
            Success
        };

        using CompletionHandler = std::function<void (Result result, HttpResponsePtr&& response)>;
        using ProgressHandler = std::function<void (uint64_t receivedBytes, uint64_t totalBytes)>;

        void perform(const HttpRequest& request, DataStream& requestBody,
            CompletionHandler completion, ProgressHandler progress = ProgressHandler());

        void perform(const HttpRequest& request, Data& requestBody,
            CompletionHandler completion, ProgressHandler progress = ProgressHandler());

        void cancel();
    };

    using HttpConnectionPtr = std::unique_ptr<HttpConnection>;

    class HttpClient final
    {
    public:
        HttpConnectionPtr connect(const std::string& domain);
    };
}
