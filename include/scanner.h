#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"
#include "util.h"

namespace calc {

class scanner {
public:
    scanner() = default;
    ~scanner() = default;
    scanner(const std::string& input);

public:
    void skip_whitespace();
    void read_char();
    char curr_char() const;
    char next_char() const;
    std::string next_string();
    std::string next_number();
    token next_token();
    
private:
    std::string input;
    std::size_t cur_idx = 0;
    std::size_t nxt_idx = 1;
    bool is_letter(char);
    bool is_digit(char);
};

} // namespace calc

#endif