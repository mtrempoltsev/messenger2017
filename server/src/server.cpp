#include "server.h"

namespace m2 {
namespace server {
    Server::Server()
    : acceptor_(io_service_),
      db(new Database("./"))
    {}

    void Server::start(uint16_t port)
    {
        auto session = std::make_shared<Session>(io_service_, db);

        // set up the acceptor to listen on the tcp port
        using boost::asio::ip::tcp;
        boost::asio::ip::tcp::endpoint endpoint(tcp::v4(), port);
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();

        acceptor_.async_accept( session->socket(), [=](auto ec)
            {
                this->handleNewConnection(session, ec);
            });

        io_service_.run();
    }

    void Server::handleNewConnection( sessionPtr session, const boost::system::error_code &error)
    {
        if (error) { return; }

        session->start();

        auto new_session = std::make_shared<Session>(io_service_, db);

        acceptor_.async_accept( new_session->socket(), [=](auto ec)
            {
                this->handleNewConnection(new_session, ec);
            });
    }
}} // m2::server
