#ifndef POSITION_STORE
#define POSITION_STORE

#include "common.hpp"
#include "Symbols.hpp"
#include <array>

#include <mutex>

using PositionSize = int64_t;
using Money = int64_t;

struct Position {
    Position() : cash(0) {}
    Money cash;
    std::array<PositionSize,NUM_SYMBOLS> positions;
};


class PositionStore {
public:
    void AddCashAndPosition(UserId user, Money cash, Symbol sym, int64_t diff) {
        std::lock_guard<std::mutex> lg(mutex_);
        auto it = positions_.emplace(
            std::piecewise_construct,  
            std::make_tuple(user), std::make_tuple());

        auto& position = it.first->second;
        position.cash += cash;
        position.positions[sym] += diff;
    }

    const Position& GetPosition(UserId user) {
        std::lock_guard<std::mutex> lg(mutex_);
        auto it = positions_.emplace(
            std::piecewise_construct,  
            std::make_tuple(user), std::make_tuple());
        return it.first->second;
    }

private:
    std::map<UserId,Position> positions_;
    std::mutex mutex_;
};


#endif