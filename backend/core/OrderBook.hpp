#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include "common.hpp"
#include "OrderBookSide.hpp"
#include "MatchingEngine.hpp"
#include <queue>


template<
    class OrderBookSide = DefaultOrderBookSide, 
    template <typename> typename MatchingEngineTemplate = DefaultMatchingEngine
>
class Orderbook {
    using MatchingEngine = MatchingEngineTemplate<OrderBookSide>;
public:
    Orderbook() : sell_side_(Sell) , buy_side_(Buy){

    }

    std::pair<Price,Price> GetBuyAndSellPrice() {
        
        Price buy = buy_side_.size() == 0 ? 0 : buy_side_.Top()->price;
        Price sell = sell_side_.size() == 0 ? 0 : sell_side_.Top()->price;
        return {buy, sell};
        
    }

    OrderEngineResult ExecuteOrder(OrderHandle handle) {
        if(handle->type == LIMIT) {
            return AddLimitOrder(handle);
        } else {//if(handle->type == EXECUTE_OR_CANCLE) {
            return ExecuteOrCancle(handle);
        }
    }

    OrderEngineResult ExecuteOrCancle(OrderHandle order) {
        OrderEngineResult result;
        result.order = std::make_unique<Order>();
        order->CopyInto(*result.order);
        if (order->side == Sell) {
            result.matching_result = MatchingEngine::Match(buy_side_, order);
        } else {
            result.matching_result = MatchingEngine::Match(sell_side_, order);
        }

        Amount traded_amount = result.matching_result.amount_traded;

        if (traded_amount == order->amount) {
            result.new_order_status = OrderUpdate {
                order->order_id, order->user_id, 
                0, order->amount, FILLED
            };
        } else if (traded_amount > 0) {
            result.new_order_status = OrderUpdate {
                order->order_id, order->user_id, 
                order->amount - traded_amount, traded_amount, PARTIALLY_FILLED
            };
            order->amount -= traded_amount;
        } else if (traded_amount == 0) {
            result.new_order_status = OrderUpdate {
                order->order_id, order->user_id, 
                0, 0, CANCLED
            };
        }
        return result;

    }

    OrderEngineResult AddLimitOrder(OrderHandle order) {
        // Return value compy optimization is going to play here for us.
        OrderEngineResult result;
        result.order = std::make_unique<Order>();
        order->CopyInto(*result.order);
        if (order->side == Sell) {
            result.matching_result = MatchingEngine::Match(buy_side_, order);
        } else {
            result.matching_result = MatchingEngine::Match(sell_side_, order);
        }

        Amount traded_amount = result.matching_result.amount_traded;

        if (traded_amount == order->amount) {
            result.new_order_status = OrderUpdate {
                order->order_id, order->user_id, 
                0, order->amount, FILLED
            };
            return result;
        } else if (traded_amount > 0) {
            result.new_order_status = OrderUpdate {
                order->order_id, order->user_id, 
                order->amount - traded_amount, traded_amount, PARTIALLY_FILLED
            };
            order->amount -= traded_amount;
        } else if (traded_amount == 0) {
            result.new_order_status = OrderUpdate {
                order->order_id, order->user_id, 
                order->amount, 0, PENDING
            };
        }

        if (order->side == Sell) {
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

