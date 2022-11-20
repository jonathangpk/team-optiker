#ifndef POSITION_STORE
#define POSITION_STORE

#include "common.hpp"
#include "Symbols.hpp"
#include <array>

#include <mutex>

using PositionSize = int64_t;
using Money = int64_t;

struct Position {
    Position() : cash(0), cash_with_commitments(0) {}
    Money cash;
    std::array<PositionSize,NUM_SYMBOLS> positions;

    // 
    Money cash_with_commitments;
    std::array<PositionSize,NUM_SYMBOLS> positions_with_commitments;
};


class PositionStore {
public:
    void GiveMoney(UserId user, Money cash) {
        auto it = positions_.emplace(
            std::piecewise_construct,  
            std::make_tuple(user), std::make_tuple());
        auto& position = it.first->second;
        position.cash += cash;
        position.cash_with_commitments += cash;
    }

    void AddCashAndPosition(UserId user, Money cash, Symbol sym, int64_t diff) {
        std::lock_guard<std::mutex> lg(mutex_);
        auto it = positions_.emplace(
            std::piecewise_construct,  
            std::make_tuple(user), std::make_tuple());

        auto& position = it.first->second;
        position.cash += cash;
        position.positions[sym] += diff;
    }

    void CancleOrder(UserId user, Money cash, Symbol sym, int64_t diff) {
        auto it = positions_.emplace(
            std::piecewise_construct,  
            std::make_tuple(user), std::make_tuple());
        auto& position = it.first->second;
        position.cash_with_commitments += cash;
        position.positions_with_commitments[sym] -= diff;
    }

    bool EnsureRich(UserId user, Money cash, Symbol sym, int64_t diff) {
        auto it = positions_.emplace(
            std::piecewise_construct,  
            std::make_tuple(user), std::make_tuple());

        auto& position = it.first->second;
        if(position.cash_with_commitments - cash >= 0 &&
            position.positions_with_commitments[sym] + diff >= 0) {
                position.cash_with_commitments -= cash;
                position.positions_with_commitments[sym] += diff;
                return true;
            }
        return false;

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