//
// Created by ovv on 05.08.17.
//
#pragma once

#include "boost/asio.hpp"
#include <string>
#include <functional>

using std::string;

namespace m2 {
namespace routing {

class NetworkConnector
{
public:
    NetworkConnector(int port, std::function<void()> onMessageReceiveCallback);
    void sendMessage(string ip, string port, char* buffer, size_t size);
    void startAccept();
    string getMyIpAddress();

private:
    std::function<void()> callback;
//    asio::socket
//    std::array<char, const_size> buffer;
};

} // end routing
} // end m2

