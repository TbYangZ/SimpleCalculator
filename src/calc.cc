#include "calc.h"

#include <numbers>

namespace calc {

int calculator::exec(int argc, char **argv) {
    if (argc == 1) return interactor();
    if (argc != 2) {
        std::println("Usage: calc [Filename]");
        return -1;
    }
    std::ifstream fin(argv[1]);
    if (!fin.is_open()) {
        std::println("Can't open the file!");
        return -1;
    }
    return file_produce(fin);
}

int calculator::interactor() {
    std::println("Welcome to my calulator!");
    const std::string promt = "> ";
    while (true) {
        std::string input;
        std::print("{}", promt);
        std::getline(std::cin, input);
        if (input == "exit") break;
        parser parse(input);
        parse.parse_progress();
    }
    return 0;
}
int calculator::file_produce(std::ifstream& fin) {
    int num_of_line = 0;
    while (!fin.eof()) {
        ++num_of_line;
        std::string line;
        std::getline(fin, line);
        parser parse(line);
        parse.parse_progress();
    }
    fin.close();
    return 0;
}

#define STR(x) #x

#define STD_UNARY_FUNC(name, oper) do { \
    auto function = std::make_shared<Function>(); \
    auto variable = std::make_shared<FunctionVar>(function, 0); \
    function->set_ptr(std::make_shared<UnaryOp<oper>>(variable)); \
    token::funcs[STR(name)] = function; \
} while (0);

calculator::calculator() {
    // intro-functions
    STD_UNARY_FUNC(sqrt, op::Sqrt);
    STD_UNARY_FUNC(sin, op::Sin);
    STD_UNARY_FUNC(cos, op::Cos);
    STD_UNARY_FUNC(tan, op::Tan);
    STD_UNARY_FUNC(arcsin, op::Arcsin);
    STD_UNARY_FUNC(arccos, op::Arccos);
    STD_UNARY_FUNC(arctan, op::Arctan);

    // intro-constant
    token::add_var("pi", std::numbers::pi_v<DType>);
    token::add_var("e" , std::numbers::e_v<DType> );
}

} // namespace calc