#ifndef ORDER_ENGINE_HPP
#define ORDER_ENGINE_HPP

#include "common.hpp"
#include "OrderBook.hpp"
#include "tools/StatusOr.h"

#include <map>
#include <string>

enum OrderEngineStatus {
  SYMBOL_NOT_FOUND
};

class OrderEngine {

    OrderEngine();

    using OrderEngineResult
        = StatusOr<MatchinEngineResult, OrderEngineStatus>;

    OrderEngineResult CreateLimitOrder(
        UserId user, const std::string& symbol, 
        Side side, Price price, Amount amount);

    OrderEngineResult CreateExecuteOrCancelOrder(
        UserId user, const std::string& symbol, 
        Side side, Price price, Amount amount);

private:
    OrderId next_order_id_; 
    std::map<std::string, Orderbook<>> symbold_to_order_book_;

};


#endif