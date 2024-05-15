#include "parser.h"

namespace calc {

std::map<std::string, DType> token::vars = {};
std::map<std::string, std::shared_ptr<Function>> token::funcs = {};

parser::parser(std::string line):
    scan(line) {
    for (auto token = scan.next_token(); token.t_type != TOKEN_TYPE::EOL; token = scan.next_token()) {
        tk_list.push_back(token);
    }
    cur = 0;
}

token parser::next_token() {
    if (cur + 1 < tk_list.size()) return tk_list[cur + 1];
    else return token(TOKEN_TYPE::EOL);
}

token parser::curr_token() {
    if (cur < tk_list.size()) return tk_list[cur];
    else return token(TOKEN_TYPE::EOL);
}

void parser::read_token() {
    if (cur < tk_list.size()) cur++;
}

EXIT_STATE parser::parse_progress() {
    if (curr_token().t_type == TOKEN_TYPE::FN) return parse_function_define();
    else if (curr_token().t_type == TOKEN_TYPE::VAR) return parse_variable_define();
    else return parse_expression();
}

EXIT_STATE parser::parse_function_define() {
    in_func_def = true;
    read_token();
    
    assert(curr_token().t_type == TOKEN_TYPE::NAME);

    std::string func_name = curr_token().tk; read_token();
    token lb = curr_token(); read_token();
    
    assert(lb.t_type == TOKEN_TYPE::LBRACKET);

    id_of_vars.clear();
    int var_count = 0;

    while (curr_token().t_type != TOKEN_TYPE::RBRACKET && curr_token().t_type != TOKEN_TYPE::EOL) {
        auto curr = curr_token();
        switch (curr.t_type) {
        case TOKEN_TYPE::NAME:
            id_of_vars[curr.tk] = var_count;
            ++var_count;
            break;
        case TOKEN_TYPE::DELIM:
            break;
        default:
            break;
        }
        read_token();
    } read_token();

    auto func = std::make_shared<Function>();
    func_ptr = func;

    lb = curr_token(); read_token();

    assert(lb.t_type == TOKEN_TYPE::EQUAL);

    state cur_state;
    while (curr_token().t_type != TOKEN_TYPE::EOL) {
        auto cur = curr_token();
        switch (cur.t_type) {
        case TOKEN_TYPE::NUMBER:
            cur_state.in_number(std::stod(cur.tk));
            break;
        case TOKEN_TYPE::ADD:
            cur_state.in_operator(TOKEN_TYPE::ADD);
            break;
        case TOKEN_TYPE::MINUS:
            cur_state.in_operator(TOKEN_TYPE::MINUS);
            break;
        case TOKEN_TYPE::MULTIPLY:
            cur_state.in_operator(TOKEN_TYPE::MULTIPLY);
            break;
        case TOKEN_TYPE::DIVIDE:
            cur_state.in_operator(TOKEN_TYPE::DIVIDE);
            break;
        case TOKEN_TYPE::LBRACKET:
            cur_state.in_operator(TOKEN_TYPE::LBRACKET);
            break;
        case TOKEN_TYPE::RBRACKET:
            cur_state.in_operator(TOKEN_TYPE::RBRACKET);
            break;
        case TOKEN_TYPE::NAME:
            if (next_token().t_type != TOKEN_TYPE::LBRACKET) {
                if (id_of_vars.count(cur.tk) != 0) cur_state.in_variable(func, id_of_vars[cur.tk]);
                else cur_state.in_number(token::vars[cur.tk]);
            } else {
                cur_state.in_function(parse_function_call());
            }
            break;
        }
        read_token();
    }
    func->set_ptr(cur_state.get_ptr());
    token::funcs[func_name] = func;
    return EXIT_STATE::SUCCESS;
}
/*
fn func(a, b, c) = ...
*/

EXIT_STATE parser::parse_variable_define() {
    read_token();
    std::string var_name = curr_token().tk; read_token();
    
    assert(curr_token().t_type == TOKEN_TYPE::EQUAL);

    read_token();
    state cur_state;
    while (curr_token().t_type != TOKEN_TYPE::EOL) {
        auto cur = curr_token();
        switch (cur.t_type) {
        case TOKEN_TYPE::NUMBER:
            cur_state.in_number(std::stod(cur.tk));
            break;
        case TOKEN_TYPE::ADD:
            cur_state.in_operator(TOKEN_TYPE::ADD);
            break;
        case TOKEN_TYPE::MINUS:
            cur_state.in_operator(TOKEN_TYPE::MINUS);
            break;
        case TOKEN_TYPE::MULTIPLY:
            cur_state.in_operator(TOKEN_TYPE::MULTIPLY);
            break;
        case TOKEN_TYPE::DIVIDE:
            cur_state.in_operator(TOKEN_TYPE::DIVIDE);
            break;
        case TOKEN_TYPE::LBRACKET:
            cur_state.in_operator(TOKEN_TYPE::LBRACKET);
            break;
        case TOKEN_TYPE::RBRACKET:
            cur_state.in_operator(TOKEN_TYPE::RBRACKET);
            break;
        case TOKEN_TYPE::NAME:
            if (next_token().t_type != TOKEN_TYPE::LBRACKET) cur_state.in_number(token::vars[cur.tk]);
            else { cur_state.in_number(parse_function_call()->value());}
            break;
        }
        read_token();
    }
    token::vars[var_name] = cur_state.value();
    return EXIT_STATE::SUCCESS;
}
/*
var x = ...
*/

EXIT_STATE parser::parse_expression() {
    in_func_def = false;
    state cur_state;
    while (curr_token().t_type != TOKEN_TYPE::EOL) {
        auto cur = curr_token();
        switch (cur.t_type) {
        case TOKEN_TYPE::NUMBER:
            cur_state.in_number(std::stod(cur.tk));
            break;
        case TOKEN_TYPE::ADD:
            cur_state.in_operator(TOKEN_TYPE::ADD);
            break;
        case TOKEN_TYPE::MINUS:
            cur_state.in_operator(TOKEN_TYPE::MINUS);
            break;
        case TOKEN_TYPE::MULTIPLY:
            cur_state.in_operator(TOKEN_TYPE::MULTIPLY);
            break;
        case TOKEN_TYPE::DIVIDE:
            cur_state.in_operator(TOKEN_TYPE::DIVIDE);
            break;
        case TOKEN_TYPE::LBRACKET:
            cur_state.in_operator(TOKEN_TYPE::LBRACKET);
            break;
        case TOKEN_TYPE::RBRACKET:
            cur_state.in_operator(TOKEN_TYPE::RBRACKET);
            break;
        case TOKEN_TYPE::NAME:
            if (next_token().t_type != TOKEN_TYPE::LBRACKET) cur_state.in_number(token::vars[cur.tk]);
            else { cur_state.in_number(parse_function_call()->value());}
            break;
        }
        read_token();
    }
    if (!cur_state.empty()) std::println("{:.2f}", cur_state.value());
    return EXIT_STATE::SUCCESS;
}

state::ptr_t parser::parse_function_call() {
    std::string func_name = curr_token().tk; read_token();
    
    assert(curr_token().t_type == TOKEN_TYPE::LBRACKET);

    read_token();
    
    std::vector<std::shared_ptr<exp_node>> value_of_vars;
    while (curr_token().t_type != TOKEN_TYPE::RBRACKET && curr_token().t_type != TOKEN_TYPE::EOL) {
        auto ptr = parse_expression_in_delim();
        if (in_func_def) value_of_vars.push_back(ptr);
        else value_of_vars.push_back(std::make_shared<Constant>(ptr->value()));
        if (curr_token().t_type == TOKEN_TYPE::DELIM) read_token();
    }
    auto func = std::make_shared<Function>(*token::funcs[func_name]);
    func->set_origin_func(token::funcs[func_name]);
    func->set_values(value_of_vars);
    return func;
}
/*
f(1, g(1,2,3), 3*(3+4))
*/

state::ptr_t parser::parse_expression_in_delim() {
    int count = 1;
    bool end = false;
    state cur_state;
    while (curr_token().t_type != TOKEN_TYPE::EOL && curr_token().t_type != TOKEN_TYPE::DELIM && !end) {
        auto cur = curr_token();
        switch (cur.t_type) {
        case TOKEN_TYPE::NUMBER:
            cur_state.in_number(std::stod(cur.tk));
            break;
        case TOKEN_TYPE::ADD:
            cur_state.in_operator(TOKEN_TYPE::ADD);
            break;
        case TOKEN_TYPE::MINUS:
            cur_state.in_operator(TOKEN_TYPE::MINUS);
            break;
        case TOKEN_TYPE::MULTIPLY:
            cur_state.in_operator(TOKEN_TYPE::MULTIPLY);
            break;
        case TOKEN_TYPE::DIVIDE:
            cur_state.in_operator(TOKEN_TYPE::DIVIDE);
            break;
        case TOKEN_TYPE::LBRACKET:
            cur_state.in_operator(TOKEN_TYPE::LBRACKET);
            count += 1;
            break;
        case TOKEN_TYPE::RBRACKET:
            count -= 1;
            if (count == 0) end = true;
            else cur_state.in_operator(TOKEN_TYPE::RBRACKET);
            break;
        case TOKEN_TYPE::NAME:
            if (next_token().t_type != TOKEN_TYPE::LBRACKET) {
                if (in_func_def && id_of_vars.count(cur.tk) != 0) cur_state.in_variable(func_ptr, id_of_vars[cur.tk]);
                else cur_state.in_number(token::vars[cur.tk]);
            } else cur_state.in_function(parse_function_call());
            break;
        case TOKEN_TYPE::DELIM:
            end = true;
            break;
        }
        if (!end) read_token();
    }
    return cur_state.get_ptr();
}

} // namespace calc