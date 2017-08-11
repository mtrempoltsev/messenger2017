#pragma once

#include <boost/asio.hpp>

#include "http_response.h"
#include "http_request.h"
#include "manager_controller.h"


namespace m2     {
namespace server {

    class Session : public std::enable_shared_from_this<Session>
    {
    public:
        Session(const Session &) = delete;


        explicit Session(boost::asio::io_service& service, Database *database);

        void start();

        boost::asio::ip::tcp::socket& socket();

    private:
        void readHeader();
        void readData(requestPtr request);
        void readHeaderDone(const boost::system::error_code & error);
        void readDataDone(const boost::system::error_code & error, requestPtr request);

        void sendResponse(responsePtr response);
        void sendResponseDone(const boost::system::error_code & error);
    private:
        boost::asio::ip::tcp::socket socket_;
        boost::asio::io_service::strand write_strand_;

        boost::asio::streambuf in_packet_;
        ManagerController manager_;

    private:
        Session &operator=(const Session &) = delete;
    };

typedef std::shared_ptr<Session> sessionPtr;
typedef std::weak_ptr  <Session> sessionWPtr;

}} // m2::server
