#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include "common.hpp"
#include <queue>


struct DefaultOrderBookSide {
public:
    DefaultOrderBookSide(Side side) : side_(side) {

    }
    void AddOrder(Order&& order) {
        orders_.push(std::move(order));
    }

    void AddOrder(const Order& order) {
        orders_.push(std::move(order));
    }

    size_t size() {
        return orders_.size();
    }

    const Order& Top() {
        return orders_.top();
    }

    void RemoveTop() {
        orders_.pop();
    }

    Side GetSide() {
        return side_;
    }
private:
    std::priority_queue<Order, std::vector<Order>> orders_;
    Side side_;
};

template<class OrderBookSide>
struct DefaultMatchingEngine {
public:
    static MatchinEngineResult Match(OrderBookSide order_book, Order order) {
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
                OrderStatus book_order_status;
                if (fill_amount == top.amount) {
                    order_book.RemoveTop();
                    removed_order = true;
                    book_order_status = Filled;
                } else if (fill_amount < top.amount) {
                    book_order_status = PartiallyFilled;
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
            }
        } while (removed_order);
        return MatchinEngineResult {
            std::move(trades), std::move(order_results), amount_traded
        };
    }
};


template<
    class OrderBookSide = DefaultOrderBookSide, 
    template <typename> typename MatchingEngineTemplate = DefaultMatchingEngine
>
class Orderbook {
    using MatchingEngine = MatchingEngineTemplate<OrderBookSide>;
public:
    Orderbook() : sell_side_(Sell) , buy_side_(Buy){

    }

    MatchinEngineResult AddLimitOrder(Order order) {
        MatchinEngineResult result;
        std::cout << "Matching" << std::endl;
        if (order.side == Sell) {
            result = MatchingEngine::Match(buy_side_, order);
        } else {
            result = MatchingEngine::Match(sell_side_, order);
        }
        std::cout << "Matched" << std::endl;

        if (result.amount_traded == order.amount) {
            OrderResult order_result {
                order.order_id, order.user_id, 
                order.amount, Filled
            };
            result.order_results.push_back(std::move(order_result));
            return result;
        } else if (result.amount_traded > 0) {
            OrderResult order_result {
                order.order_id, order.user_id, 
                order.amount, PartiallyFilled
            };
            result.order_results.push_back(std::move(order_result));
            order.amount -= result.amount_traded;
        } else if (result.amount_traded == 0) {
            OrderResult order_result {
                order.order_id, order.user_id, 
                0, Pending
            };
            result.order_results.push_back(std::move(order_result));
        }

        if (order.side == Sell) {
            sell_side_.AddOrder(std::move(order));
        } else {
            buy_side_.AddOrder(std::move(order));
        }

        return result;
    }


private:
    OrderBookSide sell_side_;
    OrderBookSide buy_side_;
};

#endif

