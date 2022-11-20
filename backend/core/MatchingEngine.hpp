
template<class OrderBookSide>
struct DefaultMatchingEngine {
public:
    static MatchinEngineResult Match(OrderBookSide& order_book, OrderHandle order) {
        TradeLogs trades;
        OrderUpdates order_updates;


        Amount amount_traded = 0;
        Amount tradeable_amount = order->amount.load();
        bool removed_order = false;
        do {
            if (order_book.size() == 0  || tradeable_amount == 0) {
                break;
            }

            

            const OrderHandle& top = order_book.Top();
            if ( (order_book.GetSide() == Buy && top->price >= order->price)
                    || (order_book.GetSide() == Sell && top->price <= order->price)) {
                Amount fill_amount = std::min(top->amount.load(), tradeable_amount);
                OrderHandle& top_mutable = const_cast<OrderHandle&>(top);
                Amount amount_before = top_mutable->amount.fetch_sub(fill_amount);
                fill_amount = std::min(amount_before, fill_amount);
                if (fill_amount == 0) {
                    order_book.RemoveTop();
                    removed_order = true;
                } else {
                    std::cout << "Orders got matched!" << std::endl;
                    amount_traded += fill_amount;
                    tradeable_amount -= fill_amount;
                    OrderStatus book_order_status = FILLED;
                    Amount left = top->amount.load();
                    if (0 >= top->amount.load()) {
                        order_book.RemoveTop();
                        removed_order = true;
                        book_order_status = FILLED;
                    } else if (0 < top->amount.load()) {
                        book_order_status = PARTIALLY_FILLED;
                        // This is save todo since it does not affect the order of
                        // our defined comparator.
                    }
                    left = std::max(left, Amount(0));
                    OrderUpdate order_update {
                        top->order_id, top->user_id, 
                        left, fill_amount, book_order_status
                    };

                    TradeLog trade {
                        top->order_id, top->user_id,
                        order->order_id, order->user_id,
                        left, top->price
                    };

                    order_updates.push_back(order_update);
                    trades.push_back(trade);
                }
            } else {
                break;
            }
        } while (removed_order && tradeable_amount > 0);
        return MatchinEngineResult {
            std::move(trades), std::move(order_updates), amount_traded
        };
    }
};