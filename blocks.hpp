#pragma once
#include <cstdint>
#include <string>

struct TOKEN {
    int16_t line;
    std::string lexeme;
};

struct IDENT : public TOKEN {};

struct INTEGER : public TOKEN {
    bool is_signed;
};

struct FLOATING : public TOKEN {
    bool is_double;
};