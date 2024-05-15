#ifndef EXP_NODE_H
#define EXP_NODE_H

#include <cmath>

#include "util.h"

namespace calc {

class exp_node {
public:
    exp_node() = default;
    virtual ~exp_node() = default;
    virtual DType value() = 0;
};

class Constant: public exp_node {
public:
    Constant(DType m_value):
        m_value(m_value) { }
    DType value() override { return m_value; }
private:
    DType m_value;
};

class Function: public exp_node {
public:
    using ptr_t = std::shared_ptr<exp_node>;
    using func_ptr_t = std::shared_ptr<Function>;
    friend class FunctionVar;
    Function() = default;
    Function(const Function& other): exp_ptr(other.exp_ptr) { }
    ~Function() = default;
    void set_values(std::vector<ptr_t> vov) { value_of_vars = vov; }
    void set_ptr(ptr_t ptr) { exp_ptr = ptr; }
    void set_origin_func(func_ptr_t origin) { origin_func = origin; }
    DType value() override ;
private:
    func_ptr_t get_origin_func(this const Function&);
    std::vector<ptr_t> value_of_vars;
    std::weak_ptr<Function> origin_func;
    ptr_t exp_ptr;
};

class FunctionVar: public exp_node {
public:
    using func_prt_t = std::shared_ptr<Function>;
    FunctionVar(func_prt_t func, int id):
        func(func), id(id) { }
    ~FunctionVar() = default;
    DType value() override { return get_func()->value_of_vars[id]->value(); }
private:
    std::shared_ptr<Function> get_func() const { return func.lock(); }
    std::weak_ptr<Function> func;
    int id;
};

template<typename Op>
class BinaryOp: public exp_node {
public:
    using ptr_t = std::shared_ptr<exp_node>;
    BinaryOp(ptr_t lhs, ptr_t rhs):
        lhs(lhs), rhs(rhs) { }
    ~BinaryOp() = default;
    DType value() override { return Op::map(lhs->value(), rhs->value()); }
private:
    std::shared_ptr<exp_node> lhs;
    std::shared_ptr<exp_node> rhs;
};

template<typename Op>
class UnaryOp: public exp_node {
public:
    using ptr_t = std::shared_ptr<exp_node>;
    UnaryOp(ptr_t lhs):
        lhs(lhs) { }
    ~UnaryOp() = default;
    DType value() override { return Op::map(lhs->value()); }
private:
    std::shared_ptr<exp_node> lhs;
};

namespace op {
struct Add { static DType map(DType a, DType b) { return a + b; } };
struct Sub { static DType map(DType a, DType b) { return a - b; } };
struct Mul { static DType map(DType a, DType b) { return a * b; } };
struct Div { static DType map(DType a, DType b) { return a / b; } };
struct Sqrt { static DType map(DType v) { return std::sqrt(v); }};
struct Sin { static DType map(DType v) { return std::sin(v); }};
struct Cos { static DType map(DType v) { return std::cos(v); }};
struct Tan { static DType map(DType v) { return std::tan(v); }};
struct Arcsin { static DType map(DType v) { return std::asin(v); }};
struct Arccos { static DType map(DType v) { return std::acos(v); }};
struct Arctan { static DType map(DType v) { return std::atan(v); }};
}



}

#endif