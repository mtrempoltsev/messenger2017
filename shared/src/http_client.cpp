#include <cassert>

#include "../include/http_client.h"

m2::HttpClient::HttpClient()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

m2::HttpClient::~HttpClient()
{
    for (auto& connection : connections_)
    {
        connection->close();
        curl_easy_cleanup(connection->curl_);
    }
    curl_global_cleanup();
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

void m2::HttpClient::perform(HttpConnection* connection, std::chrono::milliseconds timeout)
{
    connection->onPerformComplete(PerformResult::Success);
}

void m2::HttpClient::cancel(HttpConnection* connection)
{
}

#include <iostream>

int main()
{
    auto client = std::make_shared<m2::HttpClient>();

    auto connection = client->connect("https://google.com");

    m2::HttpRequest request = { "", std::chrono::milliseconds(1000) };

    connection->perform(request, std::cout,
        [client, connection](m2::PerformResult result, m2::HttpResponsePtr&& response)
        {
            std::cout << static_cast<int>(result) << std::endl;
            if (response)
            {
                std::cout << response->code << std::endl;
            }
            client->close(connection);
        },
        [](uint64_t receivedBytes, uint64_t totalBytes)
        {
            std::cout << receivedBytes << '/' << totalBytes << std::endl;
        });

    return 0;
}
