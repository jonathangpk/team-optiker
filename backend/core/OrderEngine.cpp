#include "OrderEngine.hpp"

OrderEngine::OrderEngine() {

}

OrderEngine::OrderEngineResultOrStatus OrderEngine::CreateLimitOrder(
        UserId user, const std::string& symbol, Side side, Price price, Amount amount) {


    auto symbols_it = STRING_TO_SYMBOL.find(symbol);
    if (symbols_it == STRING_TO_SYMBOL.end()) {
        // Symbol not found 
        return SYMBOL_NOT_FOUND;
    }
    
    auto sym = symbols_it->second;
    auto it = symbold_to_order_book_.find(sym);
    if(it == symbold_to_order_book_.end()) {
         // Symbol not found 
        return SYMBOL_NOT_FOUND;
    }
    auto order_handle = order_store_.CreateOrder(user, sym, side, price, amount);

    return it->second.AddLimitOrder(order_handle);
}

OrderEngine::OrderEngineResultOrStatus OrderEngine::CreateExecuteOrCancelOrder(
            UserId user, const std::string& symbol, Side side, Price price, 
            Amount amount) {
    

    auto symbols_it = STRING_TO_SYMBOL.find(symbol);
    if (symbols_it == STRING_TO_SYMBOL.end()) {
        // Symbol not found 
        return SYMBOL_NOT_FOUND;
    }
    
    auto sym = symbols_it->second;
    auto it = symbold_to_order_book_.find(sym);
    if(it == symbold_to_order_book_.end()) {
         // Symbol not found 
        return SYMBOL_NOT_FOUND;
    }

    auto order_handle = order_store_.CreateOrder(user, sym, side, price, amount);

    return it->second.ExecuteOrCancle(order_handle);
}

