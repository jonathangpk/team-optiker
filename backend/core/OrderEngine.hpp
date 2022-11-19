#ifndef ORDER_ENGINE_HPP
#define ORDER_ENGINE_HPP

#include "common.hpp"
#include "OrderBook.hpp"
#include "OrderStore.hpp"
#include "tools/StatusOr.h"

#include <map>
#include <string>
#include <mutex>
#include <deque>
#include <thread>
#include <condition_variable>
#include <functional>

enum OrderEngineStatus {
  SYMBOL_NOT_FOUND,
  ORDER_SUBMITTED
};

struct SymbolContext {
    SymbolContext (std::function<void(const OrderEngineResult&)> call_back) 
        : call_back_(call_back) ,thread_(&SymbolContext::Work, this) {

    }

    void Work() {
        while(true) {
            OrderHandle handle;
            {
                std::unique_lock<std:: mutex> lk;
                while(order_queue_.empty()) {
                    cv_.wait(lk);
                }

                handle = order_queue_.front();
                // THis was a copy so pop here.
                order_queue_.pop_front();
            }
            OrderEngineResult result = orderbook_.ExecuteOrder(handle);
        }

    }

    std::pair<Price,Price> GetBuyAndSellPrice() {
        {
            std::lock_guard<std::mutex> lg(mutex_);
            return orderbook_.GetBuyAndSellPrice();
        }
    }



    void Append(OrderHandle&& handle) {
        {
            std::lock_guard<std::mutex> lg(mutex_);
            order_queue_.emplace_back(handle);
        }
        cv_.notify_one();
    }
protected:
    std::function<void(const OrderEngineResult&)> call_back_;
    std::condition_variable cv_;
    std::mutex mutex_;
    std::deque<OrderHandle> order_queue_;
    Orderbook<> orderbook_;
    std::thread thread_;
    
};

class OrderEngine {
public:
    OrderEngine(std::function<void(const OrderEngineResult&)> call_back);

    using OrderEngineResultOrStatus
        = StatusOr<OrderEngineResult, OrderEngineStatus>;

    OrderEngineStatus CreateLimitOrder(
        UserId user, const std::string& symbol, 
        Side side, Price price, Amount amount);

    OrderEngineStatus CreateExecuteOrCancelOrder(
        UserId user, const std::string& symbol, 
        Side side, Price price, Amount amount);

    StatusOr<std::pair<Price,Price>,OrderEngineStatus>
    GetBuyAndSellPrice(const std::string& symbol);

private:
    std::vector<SymbolContext*> symbol_to_context_;


    OrderStore order_store_;

};


#endif