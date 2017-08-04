#pragma once

#include <chrono>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <curl/curl.h>

#include "perform_result.h"

namespace m2
{
    using Data = std::vector<char>;
    using DataStream = std::ostream;

    struct HttpResponse final
    {
        long code_;
        Data header_;
    };

    using HttpResponsePtr = std::unique_ptr<HttpResponse>;

    struct HttpRequest final
    {
        std::string uri_;
        std::chrono::milliseconds timeout_;
    };

    class HttpClient;

    size_t headerCallback(char* data, size_t sizeInItems, size_t bytesInItem, void* connectionPtr);
    size_t writeCallback(char* data, size_t sizeInItems, size_t bytesInItem, void* connectionPtr);
    int progressCallback(void* connectionPtr, double downloadTotal, double downloadNow, double /*uploadTotal*/, double /*uploadNow*/);

    class HttpConnection final
    {
        friend class HttpClient;
        friend size_t headerCallback(char* data, size_t sizeInItems, size_t bytesInItem, void* connectionPtr);
        friend size_t writeCallback(char* data, size_t sizeInItems, size_t bytesInItem, void* connectionPtr);
        friend int progressCallback(void* connectionPtr, double downloadTotal, double downloadedNow, double /*uploadTotal*/, double /*uploadedNow*/);
    public:
        ~HttpConnection();

        HttpConnection(const HttpConnection& copied) = delete;
        HttpConnection& operator=(const HttpConnection& copied) = delete;

        HttpConnection(HttpConnection&& movied) = delete;
        HttpConnection& operator=(HttpConnection&& movied) = delete;

        using CompletionHandler = std::function<void (PerformResult result, HttpResponsePtr&& response)>;
        using ProgressHandler = std::function<void (uint64_t receivedBytes, uint64_t totalBytes)>;

        void perform(const HttpRequest& request, DataStream& requestBody,
            CompletionHandler completion, ProgressHandler progress = ProgressHandler());

        void perform(const HttpRequest& request, Data& requestBody,
            CompletionHandler completion, ProgressHandler progress = ProgressHandler());

        void cancel();

    private:
        HttpConnection(const std::string& domain, CURL* curl, HttpClient* parent);

        using WriteFunction = std::function<void (char* data, size_t size)>;
        void perform(const HttpRequest& request, WriteFunction writeFunction,
            CompletionHandler completion, ProgressHandler progress);

        void onPerformComplete(CURLcode result);

        void close();

    private:
        const std::string domain_;

        CURL* curl_;
        HttpClient* parent_;

        CompletionHandler completion_;
        ProgressHandler progress_;

        WriteFunction writeFunction_;

        Data header_;
    };

    using HttpConnectionPtr = std::shared_ptr<HttpConnection>;
}
