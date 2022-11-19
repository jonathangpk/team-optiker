#ifndef COMMON_HPP
#define COMMON_HPP

#include <cstdint>
#include <vector>
#include <chrono>
#include <iostream>
#include <memory>
#include "Symbols.hpp"

#include <atomic>

using Price = int64_t;
using Amount = uint64_t;
using UserId = uint64_t;
using OrderId = uint64_t;

using time_point =  std::chrono::time_point<std::chrono::system_clock>;
        
enum Side {
    Sell,
    Buy
};

enum OrderType {
    LIMIT,
    EXECUTE_OR_CANCLE
};

struct Order { 
    Symbol sym;
    OrderType type;
    Side side;
    Price price;
    std::atomic<Amount> amount;
    UserId user_id;
    OrderId order_id;
    time_point order_submission;
    bool cancled;

    Order(
        Symbol sym1, OrderType type1,  Side side1, Price price1, Amount amount1, UserId user_id1,
        OrderId order_id1, time_point order_submission1, bool cancled1) :
            sym(sym1), type(type1), side(side1), price(price1), amount(amount1), 
            user_id(user_id1), order_id(order_id1), 
            order_submission(order_submission1), cancled(cancled1) {

    }


};

using OrderHandle = std::shared_ptr<Order>;

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
    FILLED,
    PARTIALLY_FILLED,
    PENDING, 
    CANCLED,
    DECREASED,
    NO_ORDER
};

struct OrderUpdate {
    OrderId order_id;
    UserId user_id;
    Amount amount_filled;
    OrderStatus status;
};


struct TradeLog {
    OrderId buy_order_id;
    UserId buy_user_id;
    OrderId sell_order_id;
    UserId sell_user_id;
    Amount amount;
    Price price;
};

using TradeLogs = std::vector<TradeLog>;
using OrderUpdates = std::vector<OrderUpdate>;

struct MatchinEngineResult {
    TradeLogs trade_logs;
    OrderUpdates order_updates;
    Amount amount_traded;
};

struct OrderEngineResult {
    MatchinEngineResult matching_result;
    OrderUpdate new_order_status;
};

#endif