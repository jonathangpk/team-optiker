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

    MatchinEngineResult ExecuteOrCancle(Order order) {
        MatchinEngineResult result;
        if (order.side == Sell) {
            result = MatchingEngine::Match(buy_side_, order);
        } else {
            result = MatchingEngine::Match(sell_side_, order);
        }

        if (result.amount_traded == order.amount) {
            OrderResult order_result {
                order.order_id, order.user_id, 
                order.amount, FILLED
            };
            result.order_results.push_back(std::move(order_result));
            
        } else if (result.amount_traded > 0) {
            OrderResult order_result {
                order.order_id, order.user_id, 
                result.amount_traded, PARTIALLY_FILLED
            };
            result.order_results.push_back(std::move(order_result));
            order.amount -= result.amount_traded;
        } else if (result.amount_traded == 0) {
            OrderResult order_result {
                order.order_id, order.user_id, 
                0, CANCLED
            };
            result.order_results.push_back(std::move(order_result));
        }
        return result;

    }

    MatchinEngineResult AddLimitOrder(Order order) {
        MatchinEngineResult result;
        if (order.side == Sell) {
            result = MatchingEngine::Match(buy_side_, order);
        } else {
            result = MatchingEngine::Match(sell_side_, order);
        }

        if (result.amount_traded == order.amount) {
            OrderResult order_result {
                order.order_id, order.user_id, 
                order.amount, FILLED
            };
            result.order_results.push_back(std::move(order_result));
            return result;
        } else if (result.amount_traded > 0) {
            OrderResult order_result {
                order.order_id, order.user_id, 
                result.amount_traded, PARTIALLY_FILLED
            };
            result.order_results.push_back(std::move(order_result));
            order.amount -= result.amount_traded;
        } else if (result.amount_traded == 0) {
            OrderResult order_result {
                order.order_id, order.user_id, 
                0, PENDING
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

