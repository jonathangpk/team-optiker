//
// Created by fooris on 19.11.22.
//
#include "ExchangeController.h"
#include "Session.h"

void session::handle_place_order(const event::PlaceOrder& order)
{
    std::cout << "handle place order\n";
    if (!cur_user_.has_value()) {
        send_error("PERMISSION DENIED. Not logged in.");
        return;
    }

    bool success = exchnage_controller_->OnOrderPlacement(
            cur_user_.value()->id,
            order.ticker(),
            convert(order.type()),
            static_cast<Money>(order.price()),
            static_cast<Amount>(order.amount())
    );

    if (!success) {
        std::cout << "Order Placement failed\n";
        send_error("Order Placement failed\n");
        return;
    }
    std::cout << "Order Placement succeeded\n";
}

