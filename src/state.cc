#include "state.h"

namespace calc {

void state::in_number(DType v) {
    node_stack.push(std::make_shared<Constant>(v));
}

void state::in_operator(op_t tk) {
    switch (tk) {
    case TOKEN_TYPE::ADD:
        while (!op_stack.empty() && op_stack.top() > TOKEN_TYPE::ADD) {
            make_node();
        }
        op_stack.push(tk);
        break;
    case TOKEN_TYPE::MINUS:
        while (!op_stack.empty() && op_stack.top() > TOKEN_TYPE::MINUS) {
            make_node();
        }
        op_stack.push(tk);
        break;
    case TOKEN_TYPE::MULTIPLY:
        while (!op_stack.empty() && op_stack.top() > TOKEN_TYPE::MULTIPLY) {
            make_node();
        }
        op_stack.push(tk);
        break;
    case TOKEN_TYPE::DIVIDE:
        while (!op_stack.empty() && op_stack.top() > TOKEN_TYPE::DIVIDE) {
            make_node();
        }
        op_stack.push(tk);
        break;
    case TOKEN_TYPE::LBRACKET:
        op_stack.push(tk);
        break;
    case TOKEN_TYPE::RBRACKET:
        while (!op_stack.empty() && op_stack.top() != TOKEN_TYPE::LBRACKET) {
            make_node();
        }
        assert(!op_stack.empty() && op_stack.top() == TOKEN_TYPE::LBRACKET);
        op_stack.pop();
        break;
    default:
        break;
    }
}

void state::in_variable(state::func_ptr_t func_prt, int id) {
    node_stack.push(std::make_shared<FunctionVar>(func_prt, id));
}

void state::in_function(state::ptr_t ptr) {
    node_stack.push(ptr);
}

DType state::value() {
    while (!op_stack.empty()) make_node();
    return node_stack.top()->value();
}

state::ptr_t state::get_ptr() {
    while (!op_stack.empty()) make_node();
    return node_stack.top();
}

void state::make_node() {
    ptr_t ptr1 = node_stack.top();
    node_stack.pop();
    ptr_t ptr2 = std::make_shared<Constant>(.0);
    if (!node_stack.empty()) {
        ptr2 = node_stack.top();
        node_stack.pop();
    }
    op_t op = op_stack.top();
    ptr_t new_node;
    op_stack.pop();
    switch (op) {
    case TOKEN_TYPE::ADD:
        new_node = std::make_shared<BinaryOp<op::Add>>(ptr2, ptr1);
        break;
    case TOKEN_TYPE::MINUS:
        new_node = std::make_shared<BinaryOp<op::Sub>>(ptr2, ptr1);
        break;
    case TOKEN_TYPE::MULTIPLY:
        new_node = std::make_shared<BinaryOp<op::Mul>>(ptr2, ptr1);
        break;
    case TOKEN_TYPE::DIVIDE:
        new_node = std::make_shared<BinaryOp<op::Div>>(ptr2, ptr1);
        break;
    default:
        break;
    }
    node_stack.push(new_node);
}

}; // namespace calc