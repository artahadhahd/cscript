#pragma once
#include <vector>
#include <string>

std::string join_vec(std::vector<std::string> in, std::string delimiter) {
    std::string out {};
    for (unsigned long int i = 0; i < in.size() - 1; ++i) {
        out += in[i];
        out += delimiter;
    }
    out += in[in.size() - 1];
    return out;
}