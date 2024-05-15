#ifndef CALC_H
#define CALC_H

#include "parser.h"

#include "util.h"

namespace calc {

class calculator {
public:
    calculator();
    int exec(int argc, char **argv);
private:
    int interactor();
    int file_produce(std::ifstream& fin);
};

}; // namespace calc

#endif