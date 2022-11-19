#ifndef SYMBOLS
#define SYMBOLS

#include<map>
#include<string>

enum Symbol : uint64_t {
    TEST,
    TSX,
    // Last Symbol dont remove this:
    NUM_SYMBOLS
};


inline static std::map<std::string,Symbol>  STRING_TO_SYMBOL = {
    {"test", TEST},
    {"TSX", TSX},
};

inline static std::map<Symbol, std::string>  SYMBOL_TO_STRING = {
        {TEST, "test"},
        {TSX, "TSX"},
};

#endif
