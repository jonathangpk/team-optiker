#include "OrderEngine.hpp"

OrderEngine::OrderEngine() {

}

OrderEngine::OrderEngineResultOrStatus OrderEngine::CreateLimitOrder(
        UserId user, const std::string& symbol, Side side, Price price, Amount amount) {


    auto order_book_it = symbold_to_order_book_.find(symbol);
    if (order_book_it == symbold_to_order_book_.end()) {
        // Symbol not found 
        return SYMBOL_NOT_FOUND;
    }
    
    auto order_handle = order_store_.CreateOrder(user, symbol, side, price, amount);
    

    auto match = order_book_it->second.AddLimitOrder(order_handle);

    return match;
}

OrderEngine::OrderEngineResultOrStatus OrderEngine::CreateExecuteOrCancelOrder(
            UserId user, const std::string& symbol, Side side, Price price, 
            Amount amount) {
    

    auto order_book_it = symbold_to_order_book_.find(symbol);
    if (order_book_it == symbold_to_order_book_.end()) {
        // Symbol not found 
        return SYMBOL_NOT_FOUND;
    }
    auto order_handle = order_store_.CreateOrder(user, symbol, side, price, amount);

    auto match = order_book_it->second.ExecuteOrCancle(order_handle);

    return match;
}

