#ifndef ORDER_ENGINE_HPP
#define ORDER_ENGINE_HPP

#include "common.hpp"
#include "OrderBook.hpp"
#include "OrderStore.hpp"
#include "tools/StatusOr.h"

#include <map>
#include <string>

enum OrderEngineStatus {
  SYMBOL_NOT_FOUND
};

class OrderEngine {

    OrderEngine();

    using OrderEngineResultOrStatus
        = StatusOr<OrderEngineResult, OrderEngineStatus>;

    OrderEngineResultOrStatus CreateLimitOrder(
        UserId user, const std::string& symbol, 
        Side side, Price price, Amount amount);

    OrderEngineResultOrStatus CreateExecuteOrCancelOrder(
        UserId user, const std::string& symbol, 
        Side side, Price price, Amount amount);

private:
    std::map<std::string, Orderbook<>> symbold_to_order_book_;
    OrderStore order_store_;

};


#endif