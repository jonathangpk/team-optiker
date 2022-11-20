#ifndef SYMBOLS
#define SYMBOLS

#include<map>
#include<string>

enum Symbol : uint64_t {
    WGX,
    AVX,
    WPT,
    VIB,
    TUM,
    ETH,
    OPK,
    // Last Symbol dont remove this:
    NUM_SYMBOLS
};


inline static std::map<std::string, Symbol>  STRING_TO_SYMBOL = {
        {"WGX", WGX},
        {"AVX", AVX},
        {"WPT", WPT},
        {"VIB", VIB},
        {"TUM", TUM},
        {"ETH", ETH},
        {"OPK", OPK},
};

inline static std::map<Symbol, std::string>  SYMBOL_TO_STRING = {
        {WGX, "WGX"},
        {AVX, "AVX"},
        {WPT, "WPT"},
        {VIB, "VIB"},
        {TUM, "TUM"},
        {ETH, "ETH"},
        {OPK, "OPK"},
};

#endif
