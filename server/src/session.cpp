#include "session.h"
#include "http_request.h"
#include "http_response.h"
#include <iostream>

void LOG(const std::string& log) {
    std::cout << log << std::endl;
}

namespace m2 {
namespace server {
    Session::Session(boost::asio::io_service& service, Database *database)
    : socket_(service)
    , write_strand_(service)
    , manager_(database, this)
    {
    }

    boost::asio::ip::tcp::socket& Session::socket()
    {
        return socket_;
    }

    void Session::sendResponse(responsePtr response)
    {
        LOG(response->toString());
        async_write( socket_, boost::asio::buffer(response->toString()), write_strand_.wrap(
            [me=shared_from_this()](const boost::system::error_code & ec, std::size_t){
                me->sendResponseDone(ec);
            }));
    }

    void Session::sendResponseDone(const boost::system::error_code & error)
    {
        readHeader();
        // socket_.close();
    }

    void Session::start()
    {
        LOG("!!! START NEW SESSION !!!");
        readHeader();
    }

    void Session::readHeader()
    {
        boost::asio::async_read_until( socket_, in_packet_, HEADER_END,
            [me=shared_from_this()](const boost::system::error_code & ec, std::size_t)
            {
                me->readHeaderDone(ec);
            });
    }

    void Session::readData(requestPtr request)
    {
        // Start reading remaining data until EOF.
        boost::asio::async_read(socket_, in_packet_, boost::asio::transfer_at_least(1),
            [me=shared_from_this(), request](const boost::system::error_code & ec, std::size_t)
            {
                me->readDataDone(ec, request);
            });
    }

    void Session::readHeaderDone(const boost::system::error_code & error)
    {
        if (error) {
            return;
        }

        std::istream stream(&in_packet_);
        auto request = std::make_shared<HttpRequest>(stream);
        int headerSize = request->getContentSize();
        int dataSize = request->getData().size();
        if (dataSize < headerSize) {
            readData(request);
        }
        else {
            readDataDone(boost::asio::error::eof, request);
        }
    }

    void Session::readDataDone(const boost::system::error_code & error, requestPtr request)
    {
        if (!error) {
            std::istream stream(&in_packet_);
            request->addData(stream);
            // Continue reading remaining data until EOF.
            int headerSize = request->getContentSize();
            int dataSize = request->getData().size();
            if (dataSize < headerSize) {
                readData(request);
            }
            else {
                readDataDone(boost::asio::error::eof, request);
            }
        }
        else if (error == boost::asio::error::eof)
        {
            auto response = manager_.doProcess(request);
            sendResponse(response);
        }
        else {
            std::cout << "Error: " << error << std::endl;
        }

    }


}} // m2::server

