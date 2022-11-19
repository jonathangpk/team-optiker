//
// Created by fooris on 19.11.22.
//

#ifndef OPTIKER_COMMON_H
#define OPTIKER_COMMON_H

#include <boost/beast/core/error.hpp>

// Report a failure
void
fail(boost::beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

#endif //OPTIKER_COMMON_H
