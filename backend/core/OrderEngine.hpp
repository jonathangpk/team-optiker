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

class OrderEngine {

    OrderEngine();

    using OrderEngineResultOrStatus
        = StatusOr<OrderEngineResult, OrderEngineStatus>;

    OrderEngineStatus CreateLimitOrder(
        UserId user, const std::string& symbol, 
        Side side, Price price, Amount amount);

    OrderEngineStatus CreateExecuteOrCancelOrder(
        UserId user, const std::string& symbol, 
        Side side, Price price, Amount amount);

private:
    

    struct SymbolContext {
        SymbolContext () : thread(&SymbolContext::Work, this) {

        }

        void Work() {
            while(true) {
                OrderHandle handle;
                {
                    std::unique_lock<std:: mutex> lk;
                    while(order_queue_.empty()) {
                        cv.wait(lk);
                    }

                    handle = order_queue_.front();
                    // THis was a copy so pop here.
                    order_queue_.pop_front();
                }
                if(handle->type == LIMIT) {
                    orderbook_.AddLimitOrder(handle);
                } else if(handle->type == EXECUTE_OR_CANCLE) {
                    orderbook_.ExecuteOrCancle(handle);
                }
            }

        }

        void Append(OrderHandle&& handle) {
            {
                std::lock_guard<std::mutex> lg(mutex_);
                order_queue_.emplace_back(handle);
            }
            cv.notify_one();
        }
    private:
        std::thread thread;
        std::condition_variable cv;
        std::mutex mutex_;
        std::deque<OrderHandle> order_queue_;
        Orderbook<> orderbook_;
    };

    std::array<SymbolContext, NUM_SYMBOLS> symbol_to_context;


    OrderStore order_store_;

};


#endif