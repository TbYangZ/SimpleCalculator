#ifndef STATE_H
#define STATE_H

#include "expression_node.h"
#include "token.h"

#include "util.h"

namespace calc {

class state {
public:
    using op_t = TOKEN_TYPE;
    using ptr_t = std::shared_ptr<exp_node>;
    using func_ptr_t = std::shared_ptr<Function>;
    void in_number(DType);
    void in_operator(op_t);
    void in_variable(func_ptr_t, int);
    void in_function(ptr_t);

    bool empty() const { return node_stack.empty(); }
    DType value();
    ptr_t get_ptr();
private:
    std::stack<op_t> op_stack;
    std::stack<ptr_t> node_stack;
    void make_node();
};

} // namespace calc

#endif