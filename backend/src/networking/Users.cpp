#include <iostream>
#include <iomanip>

#include "Users.h"

std::string get_token(uint64_t uid) {
    // I tried using SHA256 for this, but OpenSSL is the bane of my existence.
    std::ostringstream os;
    os << uid;
    return os.str();
}

// Let's just assume that there are no collisions or bad stuff...
// This is a hackathon after all.
std::string Users::register_user(const std::string& name)
{
    std::unique_lock<std::shared_mutex> w_lock(lock_);

    std::string token = get_token(cur_uid_);
    cur_uid_ += 1;
    User user {
        .id =  cur_uid_,
        .name = name,
    };

    users_.emplace(std::make_pair(token, std::move(user)));
    return token;
}

std::optional<const User*> Users::login(const std::string& token)
{
    std::shared_lock<std::shared_mutex> r_lock(lock_);

    auto it = users_.find(token);
    if (it != users_.end()) {
        return &it->second;
    }
    return {};
}
