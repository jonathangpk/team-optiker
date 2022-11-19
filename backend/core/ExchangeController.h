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
    OrderEngine order_engine_;
    PositionStore position_store_;
    static constexpr Money START_CAPITAL = 10000;

    std::map<UserId,std::shared_ptr<session>> user_sessions_;


    void ResultCallback(const OrderEngineResult &) {

    }

public:
    ExchangeController() :
    order_engine_(std::bind_front(&ExchangeController::ResultCallback, this)),
    position_store_(PositionStore())
    {
    }


    void OnNewSession(UserId userId,std::shared_ptr<session> session) {
        user_sessions_.insert({userId, std::move(session)});
    }

    bool OnOrderPlacement(UserId user, const std::string& symbol,
        Side side, Price price, Amount amount) {
        auto status = order_engine_.CreateLimitOrder(user, symbol, side, price, amount);
        return (status != SYMBOL_NOT_FOUND);
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
