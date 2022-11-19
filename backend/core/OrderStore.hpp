#ifndef ORDER_STORE_HPP
#define ORDER_STORE_HPP

#include "common.hpp"

#include <map>
#include <memory>
#include <chrono>
#include <shared_mutex>

class OrderStore {
public:
    

    OrderStore() : next_order_id_(0) {

    }

    OrderHandle CreateOrder(
            UserId user_id, OrderType type, Symbol symbol, Side side, 
            Price price, Amount  amount) {
        std::unique_lock<std::shared_mutex> w_lock(mutex_);

        OrderId order_id = ++next_order_id_;
        auto now = std::chrono::system_clock::now();

        auto order_handle = std::make_shared<Order>(
            symbol, type, side, price, amount, user_id, order_id, now, false);

        auto user_it = user_id_to_order_.emplace(
            std::piecewise_construct, std::make_tuple(user_id), std::make_tuple());
        user_it.first->second.push_back(order_handle);
        order_id_to_order_.emplace(order_id, order_handle);
        return order_handle;
    }

    const std::vector<OrderHandle>& GetOrdersOfUser(UserId user) {
        std::shared_lock<std::shared_mutex> r_lock(mutex_);
        auto it = user_id_to_order_.find(user);
        if (it == user_id_to_order_.end()) {
            return no_orders;
        } else {
            return it->second;
        }
    }


private:
    // I hate shared pointer for the the hackethon: here we go....
    using UserIdMap = std::map<UserId, std::vector<OrderHandle>>;
    using OrderIdMap = std::map<OrderId, OrderHandle>;

    UserIdMap  user_id_to_order_;
    OrderIdMap order_id_to_order_;
    const std::vector<OrderHandle> no_orders;

    OrderId next_order_id_; 
    std::shared_mutex mutex_;
    
};

#endif
