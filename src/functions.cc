#include "expression_node.h"

namespace calc {

std::shared_ptr<Function> Function::get_origin_func(this const Function& self) {
    return self.origin_func.lock();
}

DType Function::value() {
    for (auto&& ptr: value_of_vars) {
        ptr = std::make_shared<Constant>(ptr->value());
    }
    get_origin_func()->value_of_vars = this->value_of_vars;
    return exp_ptr->value();
}

} // namespace calc