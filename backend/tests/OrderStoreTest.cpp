#include <catch.hpp>

#include "../core/OrderBook.hpp"
#include "../core/OrderStore.hpp"

TEST_CASE("Order Test Store") {

    OrderStore store;
    
    {
        Price price = 100;
        Amount amount = 100;
        UserId user = 43;

        store.CreateOrder(user, LIMIT, ST0,Buy, price, amount);
        
        auto res = store.GetOrdersOfUser(user);
        CHECK(res.size() == 1);

        store.CreateOrder(user, LIMIT, ST0,Buy, price, amount);
        
        auto res2 = store.GetOrdersOfUser(user);
        CHECK(res2.size() == 2);

    }

}