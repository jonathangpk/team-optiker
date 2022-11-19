#include <catch.hpp>

#include "../core/OrderBook.hpp"

TEST_CASE("StatusOr HasNoValue") {

    Orderbook<> orderbook;
    Order order{
        Buy, 100, 100, 0, 1, time_point()
    };
    auto result = orderbook.AddLimitOrder(order);
    std::cout << result.order_results.size() << std::endl;
    CHECK(false);
}
