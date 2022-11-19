#ifndef OPTIKER_BACKEND_EXCHANGECONTROLLER_H
#define OPTIKER_BACKEND_EXCHANGECONTROLLER_H

#include <functional>

#include "ExchangeController.h"
#include "PositionStore.hpp"

class ExchangeController {
    OrderEngine order_engine_;
    PositionStore position_store_;
    const Money START_CAPITAL = 10000;



public:
    ExchangeController() :
    order_engine_(std::bind(&ExchangeController::ResultCallback, this)),
    position_store_(PositionStore()),
    {
    }



private:
    void ResultCallback(const OrderEngineResult &) {

    }

    void OnRegistion(UserId user) {
        position_store_.AddCashAndPosition(user, START_CAPITAL, 0, 0);
    }

    const Position& GetPosition(UserId user) {
       return position_store_.GetPosition(user);
    }

    void OnOrderPlacement()

};

#endif //OPTIKER_BACKEND_EXCHANGECONTROLLER_H
