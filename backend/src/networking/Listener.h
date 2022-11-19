#include <boost/beast/core.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <iostream>

#include "networking/Session.h"
#include "common.h"
#include "Users.h"
#include "ExchangeController.h"

#include <memory>
#include <functional>


class listener : public std::enable_shared_from_this<listener>
{
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    ExchangeController* exchange_controller_;
    Users* users_;

    std::function<void(std::shared_ptr<session>)> on_new_session_callback_;

public:
    listener(
            net::io_context& ioc,
            tcp::endpoint endpoint,
            std::function<void(std::shared_ptr<session>)> on_new_session_callback,
            ExchangeController* exchange_controller,
            Users* users)
            : ioc_(ioc)
            , acceptor_(ioc)
            , exchange_controller_(exchange_controller)
            , users_(users)
            , on_new_session_callback_(on_new_session_callback)
    {
        beast::error_code ec;

        // Open the acceptor
        acceptor_.open(endpoint.protocol(), ec);
        if(ec)
        {
            fail(ec, "open");
            return;
        }

        // Allow address reuse
        acceptor_.set_option(net::socket_base::reuse_address(true), ec);
        if(ec)
        {
            fail(ec, "set_option");
            return;
        }

        // Bind to the server address
        acceptor_.bind(endpoint, ec);
        if(ec)
        {
            fail(ec, "bind");
            return;
        }

        // Start listening for connections
        acceptor_.listen(
                net::socket_base::max_listen_connections, ec);
        if(ec)
        {
            fail(ec, "listen");
            return;
        }
    }

    // Start accepting incoming connections
    void
    run()
    {
        do_accept();
    }

private:
    void
    do_accept()
    {
        // The new connection gets its own strand
        acceptor_.async_accept(
                net::make_strand(ioc_),
                beast::bind_front_handler(
                        &listener::on_accept,
                        shared_from_this()));
    }

    void
    on_accept(beast::error_code ec, tcp::socket socket)
    {
        if(ec)
        {
            fail(ec, "accept");
        }
        else
        {
            // Create the session and run it
            auto ses = std::make_shared<session>(std::move(socket), exchange_controller_,  users_);
            ses->run();
            on_new_session_callback_(ses);

        }

        // Accept another connection
        do_accept();
    }
};
