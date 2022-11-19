//
// Created by fooris on 19.11.22.
//

#ifndef OPTIKER_BACKEND_USERS_H
#define OPTIKER_BACKEND_USERS_H

#include <string>
#include <boost/unordered_map.hpp>
#include <shared_mutex>

struct User {
    uint64_t id;
    std::string name;
};

const uint64_t ADMIN = 96457410983275;

class Users {
    boost::unordered_map<std::string, User> users_;
    uint64_t cur_uid_ = 0;
    std::shared_mutex lock_;

public:
    Users() {
        User admin {ADMIN, "admin"};
        users_.emplace(std::make_pair(std::string("96457410983275"), admin));
    }
    ~Users() = default;

    std::string register_user(const std::string &name);
    std::optional<const User*> login(const std::string& token);
};

#endif //OPTIKER_BACKEND_USERS_H
