#ifndef OPTIKER_BACKEND_EXCHANGECONTROLLER_H
#define OPTIKER_BACKEND_EXCHANGECONTROLLER_H

#include <functional>
class ExchangeController;
#include "Session.h"
#include "ExchangeController.h"
#include "PositionStore.hpp"
#include "OrderEngine.hpp"
#include "Symbols.hpp"
#include "Users.h"

#include <chrono>
#include <thread>

#include <optional>
#include <boost/make_shared.hpp>


#include <map>

class ExchangeController {
    OrderStore order_store_;
    OrderEngine order_engine_;
    PositionStore position_store_;
    static constexpr Money START_CAPITAL = 10000;
    std::map<UserId,std::shared_ptr<session>> user_sessions_;

    std::thread run_loop_thread_;

    static constexpr auto ms_run_loop_refresh = std::chrono::milliseconds(1000);

    void RunLoop() {
        
        // This thread is bradcasting to clients.
        while(true) {
            event::ServerMessage sm;
            auto *created = new event::ListingPrices; 
            for(auto [key, _] : STRING_TO_SYMBOL ) {
                
                auto res = order_engine_.GetBuyAndSellPrice(key);
                auto el = created->add_listings();
                el->set_symbol(key);
                el->set_bid_price(res.first);
                el->set_ask_price(res.second);
                el->set_volume(0);
            }
            sm.set_allocated_listing_updates(created);

            auto serialized = boost::make_shared<std::string const>(
                    sm.SerializeAsString());
            
            for(auto [_, sess] : user_sessions_ ) {
                sess->send(serialized);
            }


            std::this_thread::sleep_for(ms_run_loop_refresh);
        }
    }

    std::string EncodeOrders(const std::vector<OrderHandle>& orders) {
        event::ServerMessage sm;
        auto *created = new event::Orders; 
        for(const auto& order : orders) {
            event::InfoOrder* info = created->add_orders();
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
        sm.set_allocated_orders(created);
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
        std::cout <<  sm.DebugString() << std::endl;
        // {
        //     event::ClientMessage msg;
        // msg.ParseFromString(sm.SerializeAsString());
        // std::cout << "asdf" << msg.DebugString() << std::endl;
        // }
        return sm.SerializeAsString();
    }

    std::string EncodeTradeLog(TradeLog tradelog) {
        event::ServerMessage sm;
        auto *tl = new event::TradeLog;
        tl->set_buy_order_id(tradelog.buy_order_id);
        tl->set_buy_user_id(tradelog.buy_user_id);
        tl->set_sell_order_id(tradelog.sell_order_id);
        tl->set_sell_user_id(tradelog.sell_user_id);
        tl->set_amount(tradelog.amount);
        tl->set_price(tradelog.price);
        sm.set_allocated_trade_log(tl);

        

        return sm.SerializeAsString();
    }

    std::optional<std::string> EncodeOrderUpdate(const OrderUpdate& update) {
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
            pfilled->set_amount_left(update.amount_left);
            sm.set_allocated_order_partially_fullfilled(pfilled);
            break;
        }
        case CANCLED: {
            auto* canceled = new event::OrderCanceled;
            canceled->set_id(update.order_id);
            sm.set_allocated_order_canceld(canceled);
            break;
        }
        case PENDING: {
            return std::nullopt;
        } 
        case DECREASED:{
            auto* canceled_part = new event::OrderPartialyCancled;
            canceled_part->set_id(update.order_id);
            canceled_part->set_amount(update.amount_left);
            sm.set_allocated_order_decreased(canceled_part);
            break;
        }
        case NO_ORDER: {
            std::cout << "No order??" << std::endl;
            return std::nullopt;
        }    
        }
        
        return sm.SerializeAsString();
    }

    void TrySendToAdmin(std::string str) {
        auto it1 = user_sessions_.find(ADMIN);
        if (it1 != user_sessions_.end()) {
            auto sess = it1->second;

            sess->send(boost::make_shared<std::string const>(str));
        }
    }

    // void AccountUpdate(const OrderUpdate& order) {
    //     Amount amount = order.amount_filled;
    //     Price price = order.price;
    //     UserId user = order.userr_id;
    //     switch () {
    //         case FILLED:
    //         case PARTIALLY_FILLED:
    //             if()
    //             position_store_.AddCashAndPosition();

    //     }
    // }

    void ResultCallback(const OrderEngineResult &result) {
        auto it1 = user_sessions_.find(result.order->user_id);
        if (it1 != user_sessions_.end()) {
            auto sess = it1->second;

            auto enc = EncodeOrderCreated(*result.order);
             
            sess->send(boost::make_shared<std::string const>(enc));
            TrySendToAdmin(enc);

            auto opt = EncodeOrderUpdate(result.new_order_status);
            
            if (opt) {
                TrySendToAdmin(*opt);
                sess->send(boost::make_shared<std::string const>(*opt));
                // sess->send_message();
            }
            
        }

        for (auto update : result.matching_result.order_updates) {
            auto it2 = user_sessions_.find(update.user_id);
            if (it2 != user_sessions_.end()) {
                auto sess = it2->second;
                auto opt = EncodeOrderUpdate(update);
                if (opt) {
                    TrySendToAdmin(*opt);
                    sess->send(boost::make_shared<std::string const>(std::move(*opt)));
                    // sess->s<end_message();
                }
            }
        }

        // Send trade logs to admin.
        auto admin = user_sessions_.find(ADMIN);
        if (admin != user_sessions_.end()) {
            for (auto tradelog: result.matching_result.trade_logs) {
                auto sess = admin->second;
                sess->send(boost::make_shared<std::string const>(EncodeTradeLog(tradelog)));
                // sess->send_message();
            }
        }
    }

    

public:
    ExchangeController() : order_store_(),
    order_engine_(std::bind_front(&ExchangeController::ResultCallback, this), &order_store_),
    position_store_(PositionStore()), 
    run_loop_thread_(std::bind_front(&ExchangeController::RunLoop, this))
    {

        std::cout << "Exchange Created!" << std::endl;
    }


    void OnNewSession(UserId userId,std::shared_ptr<session> session) {
        // The user just connected, lets send them some swag.
        //EncodeOrders
        auto orders = order_store_.GetOrdersOfUser(userId);
        session->send(boost::make_shared<std::string const>(EncodeOrders(orders)));

        user_sessions_.insert({userId, std::move(session)});
    }

    bool OnOrderPlacement(UserId user, const std::string& symbol,
        Side side, Price price, Amount amount) {

        // auto symbols_it = STRING_TO_SYMBOL.find(symbol);
        // if (symbols_it == STRING_TO_SYMBOL.end()) {
        //     // Symbol not found 
        //     return false;
        // }
        // auto sym = symbols_it->second;

        // bool has_money = position_store_.EnsureRich(user, price * amount, sym, amount);
        // if(has_money) {
            auto status = order_engine_.CreateLimitOrder(user, symbol, side, price, amount);
            return (status != SYMBOL_NOT_FOUND);
        // }
        return false;
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

    void broadcast(std::string str) {
        auto ss = boost::make_shared<std::string>(str);
         for(auto [_, session] : user_sessions_ ) {
             session->send(ss);
         }
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
