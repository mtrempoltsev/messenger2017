//
// Created by ovv on 05.08.17.
//

#include "NetworkConnector/NetworkConnector.h"
#include <string>

using namespace m2::routing;

std::string NetworkConnector::getMyIpAddress()
{
    boost::asio::io_service io_service;

    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(boost::asio::ip::host_name(), "");
    boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(query);
    boost::asio::ip::tcp::endpoint endpoint = *it;
    return endpoint.address().to_string();
}