#include <cinttypes>


#include <array>



template <uint64_t CacheSize, uint64_t Threshold, Side side>
class Ringbuffer {
    
    void AddAtPrice(int64_t price, uint64_t amount) {

    }

    bool TryRemoveAtPrice(int64_t price, uint64_t amount) {

    }


    

private:
    uint64_t& Get(int64_t price) {
        uint64_t diff = best_price_
    }


    std::array<uint64_t, CacheSize> buffer_;
    // Prices can be negative my friend.
    int64_t best_price_;
    int64_t best_price_position_;
    static constexpr direction = side : 1 ? -1;
};



template <class T, uint64_t CacheSize>
class CachedPriorityQueue {


private:
};