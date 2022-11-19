#ifndef ORDER_BOOK_SIDE_HPP
#define ORDER_BOOK_SIDE_HPP

#include "common.hpp"
#include <queue>

struct DefaultOrderBookSide {
public:
    DefaultOrderBookSide(Side side) : side_(side) {

    }
    void AddOrder(Order&& order) {
        orders_.push(std::move(order));
    }

    void AddOrder(const Order& order) {
        orders_.push(std::move(order));
    }

    size_t size() {
        return orders_.size();
    }

    const Order& Top() {
        return orders_.top();
    }

    void RemoveTop() {
        orders_.pop();
    }

    Side GetSide() {
        return side_;
    }


private:
    std::priority_queue<Order, std::vector<Order>> orders_;
    Side side_;
};


#endif