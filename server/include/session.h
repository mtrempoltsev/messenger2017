#pragma once

#include "http_response.h"
#include "http_request.h"
#include <boost/asio.hpp>


namespace m2 {
namespace server {

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(const Session &) = delete;
    Session &operator=(const Session &) = delete;

    Session(boost::asio::io_service& service);

    void start();

    boost::asio::ip::tcp::socket& socket();

private:
    void readHeader();
    void readData(requestPtr request);
    void readHeaderDone(const boost::system::error_code & error, std::size_t bytes_transferred);
    void readDataDone(const boost::system::error_code & error, std::size_t bytes_transferred, requestPtr request);

    void sendResponse(responsePtr response);
    void sendResponseDone(const boost::system::error_code & error);
private:
    boost::asio::io_service& service_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::io_service::strand write_strand_;

    boost::asio::streambuf in_packet_;
};

typedef std::shared_ptr<Session> sessionPtr;

}} // m2::server
