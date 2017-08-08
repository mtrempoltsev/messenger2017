#pragma once

#include <boost/asio.hpp>
#include "session.h"
#include "Data/Database.h"

namespace m2 {
namespace server {
class Server 
{
public:
    Server(const Server &) = delete;
    Server &operator=(const Server &) = delete;
    Server();

    void start(uint16_t port);

private:
    void handleNewConnection( sessionPtr session, const boost::system::error_code &error);
private:
    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    Database *db;
};
}} // m2::server

