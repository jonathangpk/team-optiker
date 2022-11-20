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
#include "ExchangeController.h"
#include "common.hpp"
#include "PositionStore.hpp"

#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

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
    ExchangeController* exchnage_controller_;
    Users* users_;
    std::optional<const User*> cur_user_ = {};

    std::vector<boost::shared_ptr<std::string const>> queue_;


public:
    // Take ownership of the socket
    explicit
    session(tcp::socket&& socket, ExchangeController* exchnage_controller, Users* users)
            : ws_(std::move(socket)), exchnage_controller_(exchnage_controller)
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

        // Read a message
        do_read();
    }

    void send(boost::shared_ptr<std::string const> const & ss) {
        net::post(
            ws_.get_executor(),
            beast::bind_front_handler(
                &session::on_send,
                shared_from_this(),
                ss));
    }

    void send_error(const char* str) {
        event::ServerMessage sm;
        auto *error = new event::Error;
        error->set_error(str);
        sm.set_allocated_error(error);

        // out_ = sm.SerializeAsString();
        send(boost::make_shared<std::string const>(sm.SerializeAsString()));
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
            handle_place_order(msg.place_order());
            break;
        case event::ClientMessage::kCancelOrder:
            handle_cancle_order(msg.cancel_order());
            break;
        case event::ClientMessage::kPartialCancleOrder:
            handle_partial_cancle_order(msg.partial_cancle_order());
            break;
        case event::ClientMessage::kRegister:
            handle_register(msg.register_());
            break;
        case event::ClientMessage::kLogin:
            handle_login(msg.login());
            break;
        case event::ClientMessage::EVENT_NOT_SET:
            break;
        case event::ClientMessage::kCreateNews: {
            break;
        }
        case event::ClientMessage::kError:
            break;
        }
        in_.consume(in_.size());

        do_read();
    }

    void on_send(boost::shared_ptr<std::string const> const& ss) {
    // Always add to queue
    queue_.push_back(ss);

    // Are we already writing?
    if(queue_.size() > 1)
        return;

    // We are not currently writing, so send this immediately
    ws_.async_write(
        net::buffer(*queue_.front()),
        beast::bind_front_handler(
            &session::on_write,
            shared_from_this()));
    }
    
    void on_write(beast::error_code ec, std::size_t) {
    // Handle the error, if any
    if(ec)
        return fail(ec, "write");

    // Remove the string from the queue
    queue_.erase(queue_.begin());

    // Send the next message if any
    if(! queue_.empty())
        ws_.async_write(
            net::buffer(*queue_.front()),
            beast::bind_front_handler(
                &session::on_write,
                shared_from_this()));
    }

private:
    void handle_register(const event::Register &reg);

    void handle_login(const event::Login &login);

    Side convert(event::OrderType type) {
        switch (type) {
        case event::ASK:
            return Sell;
        case event::BID:
            return Buy;
        default:
            return Sell;
        }
    }

    void handle_place_order(const event::PlaceOrder &order);

    void handle_cancle_order(const event::CancelOrder& co);

    void handle_partial_cancle_order(const event::PartialCancelOrder& co);

    void handle_new_news(const event::ServerMessage& cn);

};


#endif //OPTIKER_SESSION_H
