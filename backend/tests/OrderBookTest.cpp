#include <catch.hpp>

#include "../core/OrderBook.hpp"

// TEST_CASE("Full Match") {

//     Orderbook<> orderbook;
    
//     {
//         Price price = 100;
//         Amount amount = 100;
//         UserId user = 43;
//         OrderId order_id = 67;
//         Order order{
//             Buy, price, amount, user, order_id, time_point()
//         };
//         auto result = orderbook.AddLimitOrder(order);
//         CHECK(result.order_results.size() == 1);
//         CHECK(result.order_results[0].amount_filled == 0);
//         CHECK(result.order_results[0].order_id == order_id);
//         CHECK(result.order_results[0].status == PENDING);
//         CHECK(result.order_results[0].user_id == user);
//     }

//     {
//         Price price = 100;
//         Amount amount = 100;
//         UserId user = 44;
//         OrderId order_id = 68;
//         Order order{
//             Sell, price, amount, user, order_id, time_point()
//         };
//         auto result = orderbook.AddLimitOrder(order);
//         CHECK(result.order_results.size() == 2);
//         CHECK(result.order_results[0].amount_filled == 100);
//         CHECK(result.order_results[0].order_id == 67);
//         CHECK(result.order_results[0].status == FILLED);
//         CHECK(result.order_results[0].user_id == 43);

//         CHECK(result.order_results[1].amount_filled == 100);
//         CHECK(result.order_results[1].order_id == order_id);
//         CHECK(result.order_results[1].status == FILLED);
//         CHECK(result.order_results[1].user_id == user);
//     }

//     {
        
//         Price price = 100;
//         Amount amount = 100;
//         UserId user = 44;
//         OrderId order_id = 68;
//         Order order{
//             Sell, price, amount, user, order_id, time_point()
//         };
//         auto result = orderbook.AddLimitOrder(order);
//         CHECK(result.order_results.size() == 1);
//         CHECK(result.order_results[0].amount_filled == 0);
//         CHECK(result.order_results[0].order_id == 68);
//         CHECK(result.order_results[0].status == PENDING);
//         CHECK(result.order_results[0].user_id == 44);
//     }
// }


// TEST_CASE("Partial Match") {

//     Orderbook<> orderbook;
    
//     {
//         Price price = 100;
//         Amount amount = 100;
//         UserId user = 43;
//         OrderId order_id = 67;
//         Order order{
//             Buy, price, amount, user, order_id, time_point()
//         };
//         auto result = orderbook.AddLimitOrder(order);
//         CHECK(result.order_results.size() == 1);
//         CHECK(result.order_results[0].amount_filled == 0);
//         CHECK(result.order_results[0].order_id == order_id);
//         CHECK(result.order_results[0].status == PENDING);
//         CHECK(result.order_results[0].user_id == user);
//     }

//     {
//         Price price = 100;
//         Amount amount = 50;
//         UserId user = 44;
//         OrderId order_id = 68;
//         Order order{
//             Sell, price, amount, user, order_id, time_point()
//         };
//         auto result = orderbook.AddLimitOrder(order);
//         CHECK(result.order_results.size() == 2);
//         CHECK(result.order_results[0].amount_filled == 50);
//         CHECK(result.order_results[0].order_id == 67);
//         CHECK(result.order_results[0].status == PARTIALLY_FILLED);
//         CHECK(result.order_results[0].user_id == 43);

//         CHECK(result.order_results[1].amount_filled == 50);
//         CHECK(result.order_results[1].order_id == order_id);
//         CHECK(result.order_results[1].status == FILLED);
//         CHECK(result.order_results[1].user_id == user);
//     }

    
//     {
//         Price price = 100;
//         Amount amount = 50;
//         UserId user = 44;
//         OrderId order_id = 68;
//         Order order{
//             Sell, price, amount, user, order_id, time_point()
//         };
//         auto result = orderbook.AddLimitOrder(order);
//         CHECK(result.order_results.size() == 2);
//         CHECK(result.order_results[0].amount_filled == 50);
//         CHECK(result.order_results[0].order_id == 67);
//         CHECK(result.order_results[0].status == FILLED);
//         CHECK(result.order_results[0].user_id == 43);

//         CHECK(result.order_results[1].amount_filled == 50);
//         CHECK(result.order_results[1].order_id == order_id);
//         CHECK(result.order_results[1].status == FILLED);
//         CHECK(result.order_results[1].user_id == user);
//     }

// }


TEST_CASE("Partial Match arriving order") {

    Orderbook<> orderbook;
    
    {
        Price price = 100;
        Amount amount = 25;
        UserId user = 43;
        OrderId order_id = 67;
        Order order{
            Buy, price, amount, user, order_id, time_point()
        };
        auto result = orderbook.AddLimitOrder(order);
        CHECK(result.order_results.size() == 1);
        CHECK(result.order_results[0].amount_filled == 0);
        CHECK(result.order_results[0].order_id == order_id);
        CHECK(result.order_results[0].status == PENDING);
        CHECK(result.order_results[0].user_id == user);
    }

    {
        Price price = 100;
        Amount amount = 50;
        UserId user = 44;
        OrderId order_id = 68;
        Order order{
            Sell, price, amount, user, order_id, time_point()
        };
        auto result = orderbook.AddLimitOrder(order);
        CHECK(result.order_results.size() == 2);
        CHECK(result.order_results[0].amount_filled == 25);
        CHECK(result.order_results[0].order_id == 67);
        CHECK(result.order_results[0].status == FILLED);
        CHECK(result.order_results[0].user_id == 43);

        CHECK(result.order_results[1].amount_filled == 25);
        CHECK(result.order_results[1].order_id == order_id);
        CHECK(result.order_results[1].status == PARTIALLY_FILLED);
        CHECK(result.order_results[1].user_id == user);
    }

    
    {
        Price price = 100;
        Amount amount = 50;
        UserId user = 44;
        OrderId order_id = 68;
        Order order{
            Sell, price, amount, user, order_id, time_point()
        };
        auto result = orderbook.AddLimitOrder(order);
        CHECK(result.order_results.size() == 1);
        CHECK(result.order_results[0].amount_filled == 0);
        CHECK(result.order_results[0].order_id == order_id);
        CHECK(result.order_results[0].status == PENDING);
        CHECK(result.order_results[0].user_id == user);
    }

    {
        Price price = 100;
        Amount amount = 26;
        UserId user = 43;
        OrderId order_id = 69;
        Order order{
            Buy, price, amount, user, order_id, time_point()
        };
        auto result = orderbook.AddLimitOrder(order);
        CHECK(result.order_results.size() == 3);
        CHECK(result.order_results[0].amount_filled == 25);
        CHECK(result.order_results[0].order_id == 68);
        CHECK(result.order_results[0].status == FILLED);
        CHECK(result.order_results[0].user_id == 44);

        CHECK(result.order_results[1].amount_filled == 1);
        CHECK(result.order_results[1].order_id == 68);
        CHECK(result.order_results[1].status == PARTIALLY_FILLED);
        CHECK(result.order_results[1].user_id == 44);

        CHECK(result.order_results[2].amount_filled == 26);
        CHECK(result.order_results[2].order_id == order_id);
        CHECK(result.order_results[2].status == FILLED);
        CHECK(result.order_results[2].user_id == user);
    }

}

