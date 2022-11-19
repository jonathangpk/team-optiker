#ifndef OPTIKER_BACKEND_EXCHANGECONTROLLER_H
#define OPTIKER_BACKEND_EXCHANGECONTROLLER_H

#include <functional>
class ExchangeController;
#include "Session.h"
#include "ExchangeController.h"
#include "PositionStore.hpp"
#include "OrderEngine.hpp"
#include "Symbols.hpp"

#include <map>

class ExchangeController {
    OrderStore order_store_;
    OrderEngine order_engine_;
    PositionStore position_store_;
    static constexpr Money START_CAPITAL = 10000;

    std::map<UserId,std::shared_ptr<session>> user_sessions_;

    std::string EncodeOrders(const std::vector<OrderHandle>& orders) {
        event::Orders sm;
        for(const auto& order : orders) {
            event::InfoOrder* info = sm.add_orders();
            info->set_id(order->order_id);
            info->set_type(order->side == Sell? event::ASK : event::BID);
            auto it = SYMBOL_TO_STRING.find(order->sym);
            if (it == SYMBOL_TO_STRING.end()) {
                std::cerr << "Symbold not in map" << std::endl;
                continue;
            }
            info->set_ticker(it->second);
            info->set_price(order->price);
            info->set_amount(order->amount.load());
        }
        return sm.SerializeAsString();
    }

    std::string EncodeOrderCreated(const Order& order) {
        event::ServerMessage sm;
        auto *created = new event::OrderCreated;
        created->set_id(order.order_id);
        created->set_type(order.side == Sell? event::ASK : event::BID);
        auto it = SYMBOL_TO_STRING.find(order.sym);
        if (it == SYMBOL_TO_STRING.end()) {
            created->set_ticker("Invalid Ticker");
        } else {
            created->set_ticker(it->second);
        }
        created->set_price(order.price);
        created->set_amount(order.amount);
        sm.set_allocated_order_created(created);

        return sm.SerializeAsString();
    }

    std::string EncodeOrderUpdate(const OrderUpdate& update) {
        event::ServerMessage sm;
        switch(update.status) {
        case FILLED: {
            auto* filled = new event::OrderFullfilled;
            filled->set_id(update.order_id);
            sm.set_allocated_order_fullfilled(filled);
            break;
        }
        case PARTIALLY_FILLED: {
            auto* pfilled = new event::OrderPartiallyFullfilled;
            pfilled->set_id(update.order_id);
            pfilled->set_amount_left(update.amount_filled);
            sm.set_allocated_order_partially_fullfilled(pfilled);
            break;
        }
        case CANCLED: {
            auto* canceled = new event::OrderCanceled;
            canceled->set_id(update.order_id);
            sm.set_allocated_order_canceld(canceled);
            break;
        }
        case PENDING:
            break;
        case DECREASED:
            break;
        case NO_ORDER:
            break;
        }
        

        return sm.SerializeAsString();
    }

    void ResultCallback(const OrderEngineResult &result) {
        auto it1 = user_sessions_.find(result.order->user_id);
        if (it1 != user_sessions_.end()) {
            auto sess = it1->second;

            sess->set_message(EncodeOrderCreated(*result.order));
            sess->send_message();

            sess->set_message(EncodeOrderUpdate(result.new_order_status));
            sess->send_message();
        }

        // TODO: trade_logs & matching_result for admin.
        for (auto update : result.matching_result.order_updates) {
            auto it2 = user_sessions_.find(update.user_id);
            if (it2 != user_sessions_.end()) {
                auto sess = it2->second;
                sess->set_message(EncodeOrderUpdate(update));
                sess->send_message();
            }
        }
    }

    

public:
    ExchangeController() : order_store_(),
    order_engine_(std::bind_front(&ExchangeController::ResultCallback, this), &order_store_),
    position_store_(PositionStore())
    {
    }


    void OnNewSession(UserId userId,std::shared_ptr<session> session) {
        // The user just connected, lets send them some swag.
        //EncodeOrders
        auto orders = order_store_.GetOrdersOfUser(userId);
        session->set_message(EncodeOrders(orders));
        session->send_message();

        user_sessions_.insert({userId, std::move(session)});
    }

    bool OnOrderPlacement(UserId user, const std::string& symbol,
        Side side, Price price, Amount amount) {
        auto status = order_engine_.CreateLimitOrder(user, symbol, side, price, amount);
        return (status != SYMBOL_NOT_FOUND);
    }

    void PartialCancleOrder(UserId user, OrderId order, Amount amount) {
        order_store_.DecreaseOrder(user, order, amount);
    }

    void CancleOrder(UserId user, OrderId order) {
        order_store_.CancelOrder(user, order);
    }


    void OnRegistion(UserId user) {
        position_store_.AddCashAndPosition(user, START_CAPITAL, static_cast<Symbol>(0), 0);
    }

private:
    const Position& GetPosition(UserId user) {
       return position_store_.GetPosition(user);
    }

    //    bool subscribe_to(const std::string &) {
//        auto it = STRING_TO_SYMBOL.find(ticker)
//        if (it == STRING_TO_SYMBOL.end()) {
//            return false;
//        }
//        it->second
//    }

};

#endif //OPTIKER_BACKEND_EXCHANGECONTROLLER_H
