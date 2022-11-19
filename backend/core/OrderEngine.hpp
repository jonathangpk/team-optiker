#ifndef ORDER_ENGINE_HPP
#define ORDER_ENGINE_HPP

#include "common.hpp"
#include "OrderBook.hpp"

#include <map>
#include <string>


class OrderEngine {

    OrderEngine();

    OrderResult CreateLimitOrder(UserId user, const std::string& symbol, Side side, Price price, Amount amount);

    //OrderId CreateExecuteorCancelOrder(UserId user, Side side, Price price, Amount amount);

private:
    OrderId next_order_id_; 
    
    std::map<std::string, Orderbook<>> symbold_to_order_book_;

};


#endif