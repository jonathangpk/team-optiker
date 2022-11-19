#include "OrderEngine.hpp"

OrderEngine::OrderEngine() : next_order_id_(0) {

}

OrderResult OrderEngine::CreateLimitOrder(
        UserId user, const std::string& symbol, Side side, Price price, Amount amount) {
    

    auto order_book_it = symbold_to_order_book_.find(symbol);
    if (order_book_it == symbold_to_order_book_.end()) {
        // Symbol not found 
        return {0, user, 0, SymbolNotFound};
    }
    OrderId order_id = ++next_order_id_;
    auto now = std::chrono::system_clock::now();
    Order order {side, price, amount, user, order_id, now};

    MatchinEngineResult match = order_book_it->second.AddLimitOrder(order);

    return {0, user, 0, SymbolNotFound};
}

//OrderId CreateExecuteorCancelOrder(UserId user, Side side, Price price, Amount amount);

