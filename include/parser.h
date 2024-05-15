#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "scanner.h"
#include "state.h"

#include "util.h"

namespace calc {

class parser {
public:
    EXIT_STATE parse_progress();
    parser() = default;
    parser(std::string);
    ~parser() = default;
    void print_tk() {
        for (auto v: tk_list) {
            std::print("{} ", v.tk);
        }
        std::puts("");
    }
private:
    std::vector<token> tk_list;
    scanner scan;
    int cur;
    bool in_func_def = false;
    std::map<std::string, int> id_of_vars;
    std::shared_ptr<Function> func_ptr;
    token next_token();
    token curr_token();
    void read_token();
    EXIT_STATE parse_expression();
    EXIT_STATE parse_function_define();
    EXIT_STATE parse_variable_define();
    state::ptr_t parse_function_call();
    state::ptr_t parse_expression_in_delim();
};

}

#endif