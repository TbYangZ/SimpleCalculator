#include "calc.h"
constexpr bool debug_mode = false;
int main(int argc, char **argv) {
    calc::calculator calc;
    if constexpr (debug_mode) {
        using namespace calc;
        parser parser1("fn func(a)=a*a"); parser1.parse_progress();
        std::println("Success sentence 1");
        parser parser2("sqrt(1.44)"); 
        parser2.print_tk();
        parser2.parse_progress();
    } else {
        return calc.exec(argc, argv);
    }
}