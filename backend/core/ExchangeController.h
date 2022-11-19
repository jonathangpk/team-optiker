#ifndef OPTIKER_BACKEND_EXCHANGECONTROLLER_H
#define OPTIKER_BACKEND_EXCHANGECONTROLLER_H

#include <functional>
class ExchangeController;
#include "Session.h"
#include "ExchangeController.h"
#include "PositionStore.hpp"
#include "OrderEngine.hpp"
#include "Symbols.hpp"

class ExchangeController {
    OrderEngine order_engine_;
    PositionStore position_store_;
    const Money START_CAPITAL = 10000;


    void ResultCallback(const OrderEngineResult &) {

    }

public:
    ExchangeController() :
    order_engine_(std::bind_front(&ExchangeController::ResultCallback, this)),
    position_store_(PositionStore())
    {
    }


    void OnNewSession(std::shared_ptr<session>) {
        std::cout << "New session Callback" << std::endl;
    }

    bool OnOrderPlacement(UserId user, const std::string& symbol,
        Side side, Price price, Amount amount) {
        auto status = order_engine_.CreateLimitOrder(user, symbol, side, price, amount);
        return (status != SYMBOL_NOT_FOUND);
    }
private:

    void OnRegistion(UserId user) {
        position_store_.AddCashAndPosition(user, START_CAPITAL, static_cast<Symbol>(0), 0);
    }

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
