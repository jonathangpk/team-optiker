
template<class OrderBookSide>
struct DefaultMatchingEngine {
public:
    static MatchinEngineResult Match(OrderBookSide& order_book, Order order) {
        TradeResults trades;
        OrderResults order_results;

        Amount amount_traded = 0;
        bool removed_order = false;
        do {
            if (order_book.size() == 0) {
                break;
            }
            const Order& top = order_book.Top();
            if ( (order_book.GetSide() == Buy && top.price >= order.price)
                    || (order_book.GetSide() == Sell && top.price <= order.price)) {
                Amount fill_amount = std::min(top.amount, order.amount);

                amount_traded += fill_amount;
                order.amount -= fill_amount;
                OrderStatus book_order_status = FILLED;
                if (fill_amount == top.amount) {
                    order_book.RemoveTop();
                    removed_order = true;
                    book_order_status = FILLED;
                } else if (fill_amount < top.amount) {
                    book_order_status = PARTIALLY_FILLED;
                    // This is save todo since it does not affect the order of
                    // our defined comparator.
                    Order& top_mutable = const_cast<Order&>(top);
                    top_mutable.amount -= fill_amount;
                }
                
                OrderResult order_result {
                    top.order_id, top.user_id, 
                    fill_amount, book_order_status
                };

                TradeResult trade {
                    top.order_id, top.user_id,
                    order.order_id, order.user_id,
                    fill_amount, top.price
                };

                order_results.push_back(order_result);
                trades.push_back(trade);
            } else {
                break;
            }
        } while (removed_order && order.amount > 0);
        return MatchinEngineResult {
            std::move(trades), std::move(order_results), amount_traded
        };
    }
};