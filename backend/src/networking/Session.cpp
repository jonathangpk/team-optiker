//
// Created by fooris on 19.11.22.
//
#include "ExchangeController.h"
#include "Session.h"

void session::handle_cancle_order(const event::CancelOrder& co) {
    OrderId id = co.id();
    exchnage_controller_->CancleOrder(cur_user_.value()->id, id);
}

void session::handle_partial_cancle_order(const event::PartialCancelOrder& co) {
    OrderId id = co.id();
    exchnage_controller_->PartialCancleOrder(cur_user_.value()->id, id, co.amount());
}

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
    
    // Give them some money:
    exchnage_controller_->OnRegistion((*cur_user_)->id);

    event::ServerMessage sm;
    auto *auth_token = new event::AuthToken;
    auth_token->set_token(std::move(token));
    sm.set_allocated_auth_token(auth_token);

    std::cout << "sending token\n";
    send(boost::make_shared<std::string const>(sm.SerializeAsString()));


    exchnage_controller_->OnNewSession(
        (*cur_user_)->id, std::move(shared_from_this()));
}

void session::handle_login(const event::Login &login) {
    cur_user_ = users_->login(login.token());
    
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
    std::cout << "serializing response.\n";
    auto s = sm.SerializeAsString();
    std::cout << "wrapping response in shared_ptr.\n";
    auto ss = boost::make_shared<std::string>(s);
    std::cout << "sending response.\n";
    send(ss);

    exchnage_controller_->OnNewSession(
        (*cur_user_)->id, std::move(shared_from_this()));
}

void session::handle_new_news(const event::CreateNews& cn) {
    if (!(cur_user_.has_value() && cur_user_.value()->id==ADMIN)) {
        event::ServerMessage sm_error;
        std::cout << "Illegal Access. User not Admin.\n";
        auto* error = new event::Error;
        error->set_error("Error: Access Denied.");
        sm_error.set_allocated_error(error);

        send(boost::make_shared<std::string const>(sm_error.SerializeAsString()));
    }
    event::ServerMessage sm;
    auto cn_c = new event::CreateNews(cn);
    sm.set_allocated_create_news(cn_c);

    exchnage_controller_->broadcast(sm.SerializeAsString());
}
