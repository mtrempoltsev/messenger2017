#include "session.h"
#include "http_request.h"
#include "http_response.h"
#include "manager_controller.h"

#include <iostream>

void LOG(const std::string& log) {
    std::cout << log << std::endl;
}

namespace m2 {
namespace server {
    Session::Session(boost::asio::io_service& service)
    : service_(service)
    , socket_(service)
    , write_strand_(service)
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
            [me=shared_from_this()](const boost::system::error_code & ec, std::size_t bytes_xfer)
            {
                me->readHeaderDone(ec, bytes_xfer);
            });
    }

    void Session::readData(requestPtr request)
    {
        // Start reading remaining data until EOF.
        boost::asio::async_read(socket_, in_packet_, boost::asio::transfer_at_least(1),
            [me=shared_from_this(), request](const boost::system::error_code & ec, std::size_t bytes_xfer)
            {
                me->readDataDone(ec, bytes_xfer, request);
            });
    }

    void Session::readHeaderDone(const boost::system::error_code & error, std::size_t bytes_transferred)
    {
        if (error) {
            return;
        }

        std::istream stream(&in_packet_);
        auto request = std::make_shared<HttpRequest>(stream);
        int headerSize = request->getFieldValue<int>(CONTENT_LENGTH);
        int dataSize = request->getData().size();
        if (dataSize < headerSize) {
            readData(request);
        }
        else {
            readDataDone(boost::asio::error::eof, 0, request);
        }
    }

    void Session::readDataDone(const boost::system::error_code & error, std::size_t bytes_transferred, requestPtr request)
    {
        if (!error) {
            std::cout << "WHAT???" << std::endl;
            std::istream stream(&in_packet_);
            request->addData(stream);
            // Continue reading remaining data until EOF.
            readData(request);
        }
        else if (error == boost::asio::error::eof)
        {
            ManagerController manager;
            auto response = manager.doProcess(request);
            sendResponse(response);
        }
        else {
            std::cout << "Error: " << error << std::endl;
        }

    }


}} // m2::server

