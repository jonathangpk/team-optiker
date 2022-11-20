#include <catch.hpp>

#include "../core/OrderBook.hpp"

TEST_CASE("Full Match") {

    Orderbook<> orderbook;
    
    {
        Price price = 100;
        Amount amount = 100;
        UserId user = 43;
        OrderId order_id = 67;
        auto order = std::make_shared<Order>(
            ST0, LIMIT ,Buy, price, amount, user, order_id, time_point()
        );
        auto result = orderbook.AddLimitOrder(order);
        CHECK(result.matching_result.order_updates.size() == 0);
        CHECK(result.new_order_status.amount_filled == 0);
        CHECK(result.new_order_status.order_id == order_id);
        CHECK(result.new_order_status.status == PENDING);
        CHECK(result.new_order_status.user_id == user);
    }

    {
        Price price = 100;
        Amount amount = 100;
        UserId user = 44;
        OrderId order_id = 68;
        auto order = std::make_shared<Order>(
            ST0, LIMIT,Sell, price, amount, user, order_id, time_point()
        );
        auto result = orderbook.AddLimitOrder(order);
        CHECK(result.matching_result.order_updates.size() == 1);
        CHECK(result.matching_result.order_updates[0].amount_filled == 0);
        CHECK(result.matching_result.order_updates[0].order_id == 67);
        CHECK(result.matching_result.order_updates[0].status == FILLED);
        CHECK(result.matching_result.order_updates[0].user_id == 43);

        CHECK(result.new_order_status.amount_filled == 100);
        CHECK(result.new_order_status.order_id == order_id);
        CHECK(result.new_order_status.status == FILLED);
        CHECK(result.new_order_status.user_id == user);
    }

    {
        
        Price price = 100;
        Amount amount = 100;
        UserId user = 44;
        OrderId order_id = 68;
        auto order = std::make_shared<Order>(
            ST0, LIMIT,Sell, price, amount, user, order_id, time_point()
        );
        auto result = orderbook.AddLimitOrder(order);
        CHECK(result.matching_result.order_updates.size() == 0);
        CHECK(result.new_order_status.amount_filled == 0);
        CHECK(result.new_order_status.order_id == 68);
        CHECK(result.new_order_status.status == PENDING);
        CHECK(result.new_order_status.user_id == 44);
    }
}


TEST_CASE("Partial Match") {

    Orderbook<> orderbook;
    
    {
        Price price = 100;
        Amount amount = 100;
        UserId user = 43;
        OrderId order_id = 67;
        auto order = std::make_shared<Order>(
            ST0, LIMIT,Buy, price, amount, user, order_id, time_point()
        );
        auto result = orderbook.AddLimitOrder(order);
        CHECK(result.matching_result.order_updates.size() == 0);
        CHECK(result.new_order_status.amount_filled == 0);
        CHECK(result.new_order_status.order_id == order_id);
        CHECK(result.new_order_status.status == PENDING);
        CHECK(result.new_order_status.user_id == user);
    }

    {
        Price price = 100;
        Amount amount = 50;
        UserId user = 44;
        OrderId order_id = 68;
        auto order = std::make_shared<Order>(
            ST0, LIMIT,Sell, price, amount, user, order_id, time_point()
        );
        auto result = orderbook.AddLimitOrder(order);
        CHECK(result.matching_result.order_updates.size() == 1);
        CHECK(result.matching_result.order_updates[0].amount_filled == 50);
        CHECK(result.matching_result.order_updates[0].order_id == 67);
        CHECK(result.matching_result.order_updates[0].status == PARTIALLY_FILLED);
        CHECK(result.matching_result.order_updates[0].user_id == 43);

        CHECK(result.new_order_status.amount_filled == 50);
        CHECK(result.new_order_status.order_id == order_id);
        CHECK(result.new_order_status.status == FILLED);
        CHECK(result.new_order_status.user_id == user);
    }

    
    {
        Price price = 100;
        Amount amount = 50;
        UserId user = 44;
        OrderId order_id = 68;
        auto order = std::make_shared<Order>(
            ST0, LIMIT,Sell, price, amount, user, order_id, time_point()
        );
        auto result = orderbook.AddLimitOrder(order);
        CHECK(result.matching_result.order_updates.size() == 1);
        CHECK(result.matching_result.order_updates[0].amount_filled == 0);
        CHECK(result.matching_result.order_updates[0].order_id == 67);
        CHECK(result.matching_result.order_updates[0].status == FILLED);
        CHECK(result.matching_result.order_updates[0].user_id == 43);

        CHECK(result.new_order_status.amount_filled == 50);
        CHECK(result.new_order_status.order_id == order_id);
        CHECK(result.new_order_status.status == FILLED);
        CHECK(result.new_order_status.user_id == user);
    }

}


TEST_CASE("Partial Match arriving order") {

    Orderbook<> orderbook;
    
    {
        Price price = 100;
        Amount amount = 25;
        UserId user = 43;
        OrderId order_id = 67;
        auto order = std::make_shared<Order>(
            ST0, LIMIT,Buy, price, amount, user, order_id, time_point()
        );
        auto result = orderbook.AddLimitOrder(order);
        CHECK(result.matching_result.order_updates.size() == 0);
        CHECK(result.new_order_status.amount_filled == 0);
        CHECK(result.new_order_status.order_id == order_id);
        CHECK(result.new_order_status.status == PENDING);
        CHECK(result.new_order_status.user_id == user);
    }

    {
        Price price = 100;
        Amount amount = 50;
        UserId user = 44;
        OrderId order_id = 68;
        auto order = std::make_shared<Order>(
            ST0, LIMIT,Sell, price, amount, user, order_id, time_point()
        );
        auto result = orderbook.AddLimitOrder(order);
        CHECK(result.matching_result.order_updates.size() == 1);
        CHECK(result.matching_result.order_updates[0].amount_filled == 0);
        CHECK(result.matching_result.order_updates[0].order_id == 67);
        CHECK(result.matching_result.order_updates[0].status == FILLED);
        CHECK(result.matching_result.order_updates[0].user_id == 43);

        CHECK(result.new_order_status.amount_filled == 25);
        CHECK(result.new_order_status.order_id == order_id);
        CHECK(result.new_order_status.status == PARTIALLY_FILLED);
        CHECK(result.new_order_status.user_id == user);
    }

    
    {
        Price price = 100;
        Amount amount = 50;
        UserId user = 44;
        OrderId order_id = 68;
        auto order = std::make_shared<Order>(
            ST0, LIMIT,Sell, price, amount, user, order_id, time_point()
        );
        auto result = orderbook.AddLimitOrder(order);
        CHECK(result.matching_result.order_updates.size() == 0);
        CHECK(result.new_order_status.amount_filled == 0);
        CHECK(result.new_order_status.order_id == order_id);
        CHECK(result.new_order_status.status == PENDING);
        CHECK(result.new_order_status.user_id == user);
    }

    {
        Price price = 100;
        Amount amount = 26;
        UserId user = 43;
        OrderId order_id = 69;
        auto order = std::make_shared<Order>(
            ST0, LIMIT,Buy, price, amount, user, order_id, time_point()
        );
        auto result = orderbook.AddLimitOrder(order);
        CHECK(result.matching_result.order_updates.size() == 2);
        CHECK(result.matching_result.order_updates[0].amount_filled == 0);
        CHECK(result.matching_result.order_updates[0].order_id == 68);
        CHECK(result.matching_result.order_updates[0].status == FILLED);
        CHECK(result.matching_result.order_updates[0].user_id == 44);

        CHECK(result.matching_result.order_updates[1].amount_filled == 49);
        CHECK(result.matching_result.order_updates[1].order_id == 68);
        CHECK(result.matching_result.order_updates[1].status == PARTIALLY_FILLED);
        CHECK(result.matching_result.order_updates[1].user_id == 44);

        CHECK(result.new_order_status.amount_filled == 26);
        CHECK(result.new_order_status.order_id == order_id);
        CHECK(result.new_order_status.status == FILLED);
        CHECK(result.new_order_status.user_id == user);
    }

}

