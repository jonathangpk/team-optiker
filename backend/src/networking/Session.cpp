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



void session::handle_register(const event::Register &reg) {
    std::cout << "handle_register\n";
    std::string token = users_->register_user(reg.name());
    cur_user_ = users_->login(token);
    exchnage_controller_->OnNewSession(
        (*cur_user_)->id, std::move(shared_from_this()));

    // Give them some money:
    exchnage_controller_->OnRegistion((*cur_user_)->id);

    event::ServerMessage sm;
    auto *auth_token = new event::AuthToken;
    auth_token->set_token(std::move(token));
    sm.set_allocated_auth_token(auth_token);

    std::cout << "sending token\n";
    out_ = sm.SerializeAsString();
    send_message();
}

void session::handle_login(const event::Login &login) {
    std::cout << "handle login\n";
    cur_user_ = users_->login(login.token());
    exchnage_controller_->OnNewSession(
        (*cur_user_)->id, std::move(shared_from_this()));

    event::ServerMessage sm;
    if (cur_user_.has_value()) {
        std::cout << "current user: " << cur_user_.value()->name << "\n";
        auto *auth_token = new event::AuthToken;
        auth_token->set_token(login.token());
        sm.set_allocated_auth_token(auth_token);
    } else {
        std::cout << "no user found for this token.\n";
        auto *error = new event::Error;
        error->set_error("Error: Invalid Token.");
        sm.set_allocated_error(error);
    }

    std::cout << "sending response.\n";
    out_ = sm.SerializeAsString();
    send_message();
}
