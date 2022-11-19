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

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class session : public std::enable_shared_from_this<session>
{
    websocket::stream<beast::tcp_stream> ws_;
    beast::flat_buffer in_buffer_;
    std::string out_buffer_;

public:
    // Take ownership of the socket
    explicit
    session(tcp::socket&& socket)
            : ws_(std::move(socket))
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
        if(ec)
            return fail(ec, "accept");

        event::ServerMessage sm;
        event::NewEvent event;
        event.set_title("Test");
        event.set_description("Testing");
        sm.set_allocated_new_event(&event);

        std::string msg = sm.SerializeAsString();
        send_message(msg);

        // Read a message
        do_read();
    }

    void send_message(std::string &msg) {
        ws_.async_write(
                boost::asio::buffer(msg),
                beast::bind_front_handler(
                        &session::on_write,
                        shared_from_this()));
    }

    void
    do_read()
    {
        // Read a message into our buffer
        ws_.async_read(
                in_buffer_,
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
        msg.ParseFromArray(in_buffer_.cdata().data(), bytes_transferred);

        std::cout << msg.place_order().ticker() << std::endl;

    }

    void
    on_write(
            beast::error_code ec,
            std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return fail(ec, "write");

        // Clear the buffer
        in_buffer_.consume(in_buffer_.size());

        // Do another read
        do_read();
    }
};


#endif //OPTIKER_SESSION_H