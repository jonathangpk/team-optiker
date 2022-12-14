#include "OrderEngine.hpp"

OrderEngine::OrderEngine(
        std::function<void(const OrderEngineResult&)>call_back, OrderStore* store ) :
        order_store_(store)  {
   for (size_t i = 0; i < NUM_SYMBOLS; i++) {
       symbol_to_context_.push_back(new SymbolContext(call_back));
   }

}

OrderEngine::~OrderEngine() {
    for (size_t i = 0; i < NUM_SYMBOLS; i++) {
        delete symbol_to_context_[i];
    }

}

std::pair<Price,Price>
OrderEngine::GetBuyAndSellPrice(const std::string& symbol) {
    auto symbols_it = STRING_TO_SYMBOL.find(symbol);
    if (symbols_it == STRING_TO_SYMBOL.end()) {
        // Symbol not found 
        return {0,0};
    }
    auto sym = symbols_it->second;
    return symbol_to_context_[sym]->GetBuyAndSellPrice();
}

OrderEngineStatus OrderEngine::CreateLimitOrder(
        UserId user, const std::string& symbol, Side side, Price price, Amount amount) {


    auto symbols_it = STRING_TO_SYMBOL.find(symbol);
    if (symbols_it == STRING_TO_SYMBOL.end()) {
        // Symbol not found 
        return SYMBOL_NOT_FOUND;
    }
    auto sym = symbols_it->second;
    auto order_handle = order_store_->CreateOrder(user, LIMIT, sym, side, price, amount);
    symbol_to_context_[sym]->Append(std::move(order_handle));

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
    auto order_handle = order_store_->CreateOrder(user, EXECUTE_OR_CANCLE, sym, side, price, amount);
    symbol_to_context_[sym]->Append(std::move(order_handle));

    return ORDER_SUBMITTED;
}

