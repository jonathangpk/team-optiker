#ifndef COMMON_HPP
#define COMMON_HPP

#include <cstdint>
#include <vector>
#include <chrono>
#include <iostream>

using Price = int64_t;
using Amount = uint64_t;
using UserId = uint64_t;
using OrderId = uint64_t;

using time_point =  std::chrono::time_point<std::chrono::system_clock>;
        
enum Side {
    Sell,
    Buy
};

struct Order {
    Side side;
    Price price;
    Amount amount;
    UserId user_id;
    OrderId order_id;
    time_point order_submission;
};

inline bool operator<(const Order& lhs, const Order& rhs) {
    if (lhs.side != rhs.side) {
        // We should never do these comparisoinsn
        std::cerr << "Orders of two sides are compared" << std::endl;
        return true;
    }
    if (lhs.price == rhs.price) {
        // The timestamp comparision is imporntant s.t. older queries get
        // matched first
        return lhs.order_submission > rhs.order_submission;
    } else if (lhs.side == Buy) {
        return lhs.price < rhs.price;
    }
    // This is now the SellSide
    return lhs.price > rhs.price;
}

enum OrderStatus {
    Filled,
    PartiallyFilled,
    Pending, 
    SymbolNotFound
};

struct OrderResult {
    OrderId order_id;
    UserId user_id;
    Amount amount_filled;
    OrderStatus status;
};

struct TradeResult {
    OrderId buy_order_id;
    UserId buy_user_id;
    OrderId sell_order_id;
    UserId sell_user_id;
    Amount amount;
    Price price;
};

using TradeResults = std::vector<TradeResult>;
using OrderResults = std::vector<OrderResult>;

struct MatchinEngineResult {
    TradeResults trade_results;
    OrderResults order_results;
    Amount amount_traded;
};

#endif