#ifndef SYMBOLS
#define SYMBOLS

#include<map>
#include<string>

enum Symbol : uint64_t {
    ST0,
    ST1,
    ST2,
    ST3,
    ST4,
    ST5,
    ST6,
    ST7,
    ST8,
    ST9
    // Last Symbol dont remove this:
    NUM_SYMBOLS
};


inline static std::map<std::string, Symbol>  STRING_TO_SYMBOL = {
        {"Test Stock 0", ST0},
        {"Test Stock 1", ST1},
        {"Test Stock 2", ST2},
        {"Test Stock 3", ST3},
        {"Test Stock 4", ST4},
        {"Test Stock 5", ST5},
        {"Test Stock 6", ST6},
        {"Test Stock 7", ST7},
        {"Test Stock 8", ST8},
        {"Test Stock 9", ST9},
};

inline static std::map<Symbol, std::string>  SYMBOL_TO_STRING = {
        {ST0, "Test Stock 0"},
        {ST1, "Test Stock 1"},
        {ST2, "Test Stock 2"},
        {ST3, "Test Stock 3"},
        {ST4, "Test Stock 4"},
        {ST5, "Test Stock 5"},
        {ST6, "Test Stock 6"},
        {ST7, "Test Stock 7"},
        {ST8, "Test Stock 8"},
        {ST9, "Test Stock 9"},
};

#endif
