#ifndef SYMBOLS
#define SYMBOLS

#include<map>
#include<string>

enum Symbol : uint64_t {
    TEST,
    // Last Symbol dont remove this:
    NUM_SYMBOLS
};


inline static std::map<std::string,Symbol>  STRING_TO_SYMBOL = {
    {"test", TEST}
};

#endif