#ifndef ORDER_BOOK_SIDE_HPP
#define ORDER_BOOK_SIDE_HPP

#include "common.hpp"
#include <queue>

struct DefaultOrderBookSide {
public:
    DefaultOrderBookSide(Side side) : side_(side) {

    }
    void AddOrder(OrderHandle&& order) {
        orders_.push(std::move(order));
    }

    void AddOrder(const OrderHandle& order) {
        orders_.push(std::move(order));
    }

    size_t size() {
        return orders_.size();
    }

    const OrderHandle& Top() {
        return orders_.top();
    }

    void RemoveTop() {
        orders_.pop();
    }

    Side GetSide() {
        return side_;
    }


private:
    std::priority_queue<OrderHandle, std::vector<OrderHandle>> orders_;
    Side side_;
};


#endif