#include "scanner.h"

namespace calc {

scanner::scanner(const std::string& input): input(input) { }

void scanner::skip_whitespace() {
    while (curr_char() == ' ' || curr_char() == '\n' || curr_char() == '\t' || curr_char() == '\r')
        read_char();
}

void scanner::read_char() {
    if (cur_idx < input.size()) {
        ++cur_idx;
        nxt_idx = cur_idx + 1;
    }
}

char scanner::curr_char() const {
    if (cur_idx < input.size()) return input[cur_idx];
    else return 0;
}

char scanner::next_char() const {
    if (nxt_idx < input.size()) return input[nxt_idx];
    else return 0;
}

std::string scanner::next_string() {
    std::string str;
    while (is_letter(curr_char())) {
        str.push_back(curr_char());
        read_char();
    }
    return str;
}

std::string scanner::next_number() {
    std::string str;
    bool dot = false;
    while (is_digit(curr_char()) || curr_char() == '.') {
        if (curr_char() == '.') {
            if (dot) break;
            dot = true;
        }
        str.push_back(curr_char());
        read_char();
    }
    return str;
}

token scanner::next_token() {
    skip_whitespace();
    char curr = curr_char();
    switch (curr) {
    case '+': read_char(); return token(TOKEN_TYPE::ADD, "+"); break;
    case '-': read_char(); return token(TOKEN_TYPE::MINUS, "-"); break;
    case '*': read_char(); return token(TOKEN_TYPE::MULTIPLY, "*"); break;
    case '/': read_char(); return token(TOKEN_TYPE::DIVIDE, "/"); break;
    case '(': read_char(); return token(TOKEN_TYPE::LBRACKET, "("); break;
    case ')': read_char(); return token(TOKEN_TYPE::RBRACKET, ")"); break;
    case '=': read_char(); return token(TOKEN_TYPE::EQUAL, "="); break;
    case ',': read_char(); return token(TOKEN_TYPE::DELIM, ","); break;
    default:
        if (is_letter(curr_char())) {
            auto str = next_string();
            if (str == "fn") return token(TOKEN_TYPE::FN, "FN");
            if (str == "var") return token(TOKEN_TYPE::VAR, "VAR");
            return token(TOKEN_TYPE::NAME, str);
        }
        if (is_digit(curr_char()) || curr_char() == '.') return token(TOKEN_TYPE::NUMBER, next_number());
        return token(TOKEN_TYPE::EOL, "EOL");
        break;
    }
}

bool scanner::is_letter(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

bool scanner::is_digit(char ch) {
    return ch >= '0' && ch <= '9';
}

} // namespace calc