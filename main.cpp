#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <optional>

#include "blocks.hpp"
#include "utils.hpp"

enum class KEYWORDS : uint8_t {
    NOTAKEYWORD,
    Static,
    Volatile,
    Int,
    Unsigned,
    Signed,
    Register,
    Long,
    Double,
    Float,
    Char,
    Short,
    Const,
};

using var_t = std::pair<std::string, std::vector<KEYWORDS>>;

std::unordered_map<std::string, KEYWORDS> get_keyword {
    {"static", KEYWORDS::Static},
    {"volatile", KEYWORDS::Volatile},
    {"int", KEYWORDS::Int},
    {"unsigned", KEYWORDS::Unsigned},
    {"signed", KEYWORDS::Signed},
    {"register", KEYWORDS::Register},
    {"long", KEYWORDS::Long},
    {"double", KEYWORDS::Double},
    {"float", KEYWORDS::Float},
    {"char", KEYWORDS::Char},
    {"short", KEYWORDS::Short},
    {"const", KEYWORDS::Const}
};

std::string read_test_file() {
    std::string out {};
    std::string line;
    std::ifstream test("test.c");
    if (!test.is_open()) {
        std::cerr << "Couldn't read test file\n";
        exit(1);
    }
    while (std::getline(test, line)) {
        out += line;
        out += "\n";
    }
    test.close();
    return out;
}

static bool isidentchar(char c)
{
    switch (c) {
        case 'a' ... 'z':
        case 'A' ... 'Z':
        case '0' ... '9':
        case '_':
        return true;
    }
    return false;
}

class Parser {
public:
    Parser(std::string input_) : input(input_) {
        this->line = 1;
        this->cursor = 0;
        this->bsize = input.length();
    }

    void run() {
        if (!parse_typedef()) {
            std::cout << "Failed to parse typedef";
        }

        if (!parse_typedef()) {
            std::cout << "Failed to parse typedef";
        }
    }
private:
    constexpr bool has() const {
        return cursor < bsize;
    }

    void parse_line() {
        while (has() && input.at(cursor) == '\n') {
            ++this->line;
            ++cursor;
        }
    }

    void skip_whitespace() {
        while (has() && input.at(cursor) == ' ') {
            ++cursor;
        }
        while (has()) {
            if (parse_symbol("//")) {
                while (has() && input.at(cursor) != '\n') {
                    ++cursor;
                }
                parse_line();
            } else {
                break;
            }
        }
        parse_line();
    }

    void exit_forcefully(std::string_view message) {
        std::cerr << "Line " << line << ": " << message << '\n';
        exit(1);
    }

    bool parse_typedef() {
        skip_whitespace();
        if (!parse_symbol("typedef")) {
            return false;
        }
        std::vector<std::string> assignee;
        while (!parse_symbol(";")) {
            assignee.push_back(parser_identifier());
        }
        const std::string typedef_name = assignee.back();
        if (get_keyword[typedef_name] != KEYWORDS::NOTAKEYWORD) {
            exit_forcefully("Couldn't parse typedef: ill-formed");
        }
        assignee.pop_back();
        for (auto a : assignee) {
            if (get_keyword[a] == KEYWORDS::NOTAKEYWORD) {
                if (typedef_pool[a].size() == 0) {
                    exit_forcefully("Not a type in typedef");
                }
                assignee.pop_back();
                for (auto v : typedef_pool[a]) {
                    assignee.push_back(v);
                }
            }
        }
        typedef_pool[typedef_name] = assignee;
        return true;
    }

    std::string parser_identifier() {
        skip_whitespace();
        std::string out = "";
        while (has() && isidentchar(input.at(cursor))) {
            out += input.at(cursor);
            ++cursor;
        }
        return out;
    }

    bool parse_symbol(std::string symbol) {
        // basically strcmp
        while (has() && input.at(cursor) == ' ') {
            ++cursor;
        }
        size_t i = 0;
        while (has() && i < symbol.length()) {
            if (input.at(cursor) != symbol.at(i)) {
                cursor -= i;
                return false;
            }
            ++i;
            ++cursor;
        }
        return true;
    }

    std::string parse_integer() {
        skip_whitespace();
        if (input.at(cursor) == '0') {
            cursor++;
            return "0";
        }
        std::string out {};
        while (isdigit(input.at(cursor))) {
            out += input.at(cursor);
            ++cursor;
        }
        return out;
    }
    std::string input;
    uint64_t cursor, bsize;
    uint16_t line;
    uint8_t current_scope = 0;
protected:
    std::unordered_map<std::string, std::vector<std::string>> typedef_pool;
};

int main()
{
    const std::string test_file = read_test_file();
    Parser parser = Parser ( test_file );
    parser.run();
}