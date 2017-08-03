#pragma once

#include <chrono>
#include <string>
#include <unordered_set>

#include <curl/curl.h>

#include "http_connection.h"

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
    class HttpClient final
    {
        friend class HttpConnection;
    public:
        HttpClient();
        ~HttpClient();

        HttpConnectionPtr connect(const std::string& domain);
        void close(HttpConnectionPtr connection);

    private:
        void perform(HttpConnection* connection, std::chrono::milliseconds timeout);
        void cancel(HttpConnection* connection);

    private:
        std::unordered_set<HttpConnectionPtr> connections_;
    };
}
