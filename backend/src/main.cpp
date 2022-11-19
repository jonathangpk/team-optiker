#include <boost/beast/core.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <iostream>

#include "ExchangeController.h"
#include "networking/Session.h"
#include "networking/Listener.h"
#include "common.h"
#include "Users.h"




//------------------------------------------------------------------------------


// Echoes back all received WebSocket messages

//------------------------------------------------------------------------------

// Accepts incoming connections and launches the sessions

//------------------------------------------------------------------------------


int main(int argc, char* argv[])
{
    // Check command line arguments.
    if (argc != 4)
    {
        std::cerr <<
                  "Usage: websocket-server-async <address> <port> <threads>\n" <<
                  "Example:\n" <<
                  "    websocket-server-async 0.0.0.0 8080 1\n";
        return EXIT_FAILURE;
    }
    auto const address = net::ip::make_address(argv[1]);
    auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto const threads = std::max<int>(1, std::atoi(argv[3]));

    Users users;

    // The io_context is required for all I/O
    net::io_context ioc{threads};

    ExchangeController exchange_controller;

    // Create and launch a listening port
    std::make_shared<listener>(ioc, tcp::endpoint{address, port}, 
        std::bind_front(&ExchangeController::OnNewSession,exchange_controller), 
        &exchange_controller, &users)->run();

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for(auto i = threads - 1; i > 0; --i)
        v.emplace_back(
                [&ioc]
                {
                  ioc.run();
                });
    ioc.run();

    return EXIT_SUCCESS;
}

