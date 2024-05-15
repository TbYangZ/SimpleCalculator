#ifndef TOKEN_H
#define TOKEN_H

#include "expression_node.h"

#include "util.h"

namespace calc {

enum class TOKEN_TYPE {
    LBRACKET,
    RBRACKET,
    ADD,
    MINUS,
    MULTIPLY,
    DIVIDE,
    EQUAL,
    NAME,
    NUMBER,
    FN,
    VAR,
    DELIM,
    EOL
};

struct token {
    TOKEN_TYPE t_type;  // token type
    std::string tk;     // token
    static std::map<std::string, DType> vars;
    static std::map<std::string, std::shared_ptr<Function>> funcs;
    static void add_var(std::string name, DType value) {
        vars[name] = value;
    }
    token() = default;
    token(TOKEN_TYPE t_type): t_type(t_type) {}
    token(TOKEN_TYPE t_type, std::string tk):
        t_type(t_type), tk(tk) {}
};

}

#endif