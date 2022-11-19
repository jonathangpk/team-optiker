#include "OrderEngine.hpp"

OrderEngine::OrderEngine() {
    // for (const auto&[_, _] : STRING_TO_SYMBOL) {

    // }

}

OrderEngineStatus OrderEngine::CreateLimitOrder(
        UserId user, const std::string& symbol, Side side, Price price, Amount amount) {


    auto symbols_it = STRING_TO_SYMBOL.find(symbol);
    if (symbols_it == STRING_TO_SYMBOL.end()) {
        // Symbol not found 
        return SYMBOL_NOT_FOUND;
    }
    auto sym = symbols_it->second;
    auto order_handle = order_store_.CreateOrder(user, LIMIT, sym, side, price, amount);
    symbol_to_context[sym].Append(std::move(order_handle));

    return ORDER_SUBMITTED;
}

OrderEngineStatus OrderEngine::CreateExecuteOrCancelOrder(
            UserId user, const std::string& symbol, Side side, Price price, 
            Amount amount) {
    

    auto symbols_it = STRING_TO_SYMBOL.find(symbol);
    if (symbols_it == STRING_TO_SYMBOL.end()) {
        // Symbol not found 
        return SYMBOL_NOT_FOUND;
    }
    
    auto sym = symbols_it->second;
    auto order_handle = order_store_.CreateOrder(user, EXECUTE_OR_CANCLE, sym, side, price, amount);
    symbol_to_context[sym].Append(std::move(order_handle));

    return ORDER_SUBMITTED;
}

