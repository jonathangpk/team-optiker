#ifndef OPTIKER_SESSION_H
#define OPTIKER_SESSION_H

// TODO: Fix includes and move implementation to its own compilation unit.
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <boost/beast/websocket.hpp>
#include <string>

#include "common.h"
#include "events.pb.h"
#include "Users.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class session : public std::enable_shared_from_this<session>
{
    websocket::stream<beast::tcp_stream> ws_;
    beast::flat_buffer in_;
    std::string out_;
    Users* users_;
    std::optional<const User*> cur_user_ = {};


public:
    // Take ownership of the socket
    explicit
    session(tcp::socket&& socket, Users* users)
            : ws_(std::move(socket))
            , users_(users)
    {
    }

    // Get on the correct executor
    void
    run()
    {
        // We need to be executing within a strand to perform async operations
        // on the I/O objects in this session. Although not strictly necessary
        // for single-threaded contexts, this example code is written to be
        // thread-safe by default.
        net::dispatch(ws_.get_executor(),
                beast::bind_front_handler(
                        &session::on_run,
                        shared_from_this()));
    }

    // Start the asynchronous operation
    void
    on_run()
    {
        // Set suggested timeout settings for the websocket
        ws_.set_option(
                websocket::stream_base::timeout::suggested(
                        beast::role_type::server));

        // Set a decorator to change the Server of the handshake
        ws_.set_option(websocket::stream_base::decorator(
                [](websocket::response_type& res)
                {
                  res.set(http::field::server,
                          std::string(BOOST_BEAST_VERSION_STRING) +
                                  " websocket-server-async");
                }));
        // Accept the websocket handshake
        ws_.async_accept(
                beast::bind_front_handler(
                        &session::on_accept,
                        shared_from_this()));
    }

    void
    on_accept(beast::error_code ec)
    {
        ws_.binary(true);
        if(ec)
            return fail(ec, "accept");

//        event::ServerMessage sm;
//        event::NewEvent *event = new event::NewEvent;
//        event->set_title("Test");
//        event->set_description("Testing");
//        event->add_actions();
//        sm.set_allocated_new_event(event);
//
//        out_ = sm.SerializeAsString();
//
//        send_message();

        // Read a message
        do_read();
    }

    void send_message() {
        ws_.async_write(
                net::buffer(out_),
                beast::bind_front_handler(
                        &session::on_write,
                        shared_from_this()));
    }

    void send_error(const char* str) {
        event::ServerMessage sm;
        auto *error = new event::Error;
        error->set_error(str);
        sm.set_allocated_error(error);

        out_ = sm.SerializeAsString();
        send_message();
    }

    void
    do_read()
    {
        // Read a message into our buffer
        ws_.async_read(
                in_,
                beast::bind_front_handler(
                        &session::on_read,
                        shared_from_this()));
    }

    void
    on_read(
            beast::error_code ec,
            std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        // This indicates that the session was closed
        if(ec == websocket::error::closed)
            return;

        if(ec)
            return fail(ec, "read");

        event::ClientMessage msg;
        msg.ParseFromArray(in_.cdata().data(), bytes_transferred);

        switch (msg.event_case()) {
        case event::ClientMessage::kSubscribeListing:
            break;
        case event::ClientMessage::kUnsubscribeListing:
            break;
        case event::ClientMessage::kPlaceOrder:
//            handle_place_order(msg.place_order());
            break;
        case event::ClientMessage::kCancelOrder:
            break;
        case event::ClientMessage::kRegister:
            handle_register(msg.register_());
            break;
        case event::ClientMessage::kLogin:
            handle_login(msg.login());
            break;
        case event::ClientMessage::EVENT_NOT_SET:
            break;
        case event::ClientMessage::kError:
            break;
        }

        do_read();
    }

    void
    on_write(beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return fail(ec, "write");
    }

private:
    void handle_register(const event::Register &reg) {
        std::cout << "handle_register\n";
        std::string token = users_->register_user(reg.name());

        event::ServerMessage sm;
        auto *auth_token = new event::AuthToken;
        auth_token->set_token(std::move(token));
        sm.set_allocated_auth_token(auth_token);

        std::cout << "sending token\n";
        out_ = sm.SerializeAsString();
        send_message();
    }

    void handle_login(const event::Login &login) {
        std::cout << "handle login\n";
        cur_user_ = users_->login(login.token());

        event::ServerMessage sm;
        if (cur_user_.has_value()) {
            std::cout << "current user: " << cur_user_.value()->name << "\n";
            auto *auth_token = new event::AuthToken;
            auth_token->set_token(login.token());
            sm.set_allocated_auth_token(auth_token);
        } else {
            std::cout << "no user found for this token.\n";
            auto *error = new event::Error;
            error->set_error("Error: Invalid Token.");
            sm.set_allocated_error(error);
        }

        std::cout << "sending response.\n";
        out_ = sm.SerializeAsString();
        send_message();
    }

//    void handle_place_order(const event::PlaceOrder & order) {
//        std::cout << "handle place order\n";
//        if (!cur_user_.has_value()) {
//            send_error("PERMISSION DENIED. Not logged in.");
//            return;
//        }
//
//  //      TODO: Do call.
//        event::ServerMessage sm;
//        out_ = sm.SerializeAsString();
//        send_message();
//    }
};


#endif //OPTIKER_SESSION_H
