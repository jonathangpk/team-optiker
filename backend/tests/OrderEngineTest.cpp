



#include <catch.hpp>

#include "../core/OrderEngine.hpp"

void getResultCallback(const OrderEngineResult& ) {
    std::cout << "traded" << std::endl;
}

TEST_CASE("Engine") {

//     std::vector<SymbolContext*> symbol_to_context_;

// //     for (size_t i = 0; i < NUM_SYMBOLS; i++) {
// //        symbol_to_context_.push_back(new SymbolContext(&getResultCallback));
// //    }

// //    for (size_t i = 0; i < NUM_SYMBOLS; i++) {
// //         delete symbol_to_context_[i];
// //     }

//     OrderEngine eng(getResultCallback);

//     eng.CreateLimitOrder(0, std::string("test"), Sell, 100, 100);

//     eng.CreateLimitOrder(0, std::string("test"), Buy, 100, 100);


}