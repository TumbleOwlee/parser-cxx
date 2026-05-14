#include <iostream>

#include "parser/parser.h"

int main(int argc, char **argv) {
    // Invalid statement: "iif" is not a valid keyword.
    {
        char const *const STATEMENT = R"x(
        if (1.1 == 1) {
            iif (3 == 3) {
                11
            } else {
                22
            }
        } else {
            33
        }
        )x";
        std::cerr << parse::parse(STATEMENT) << std::endl;
    }
    // Valid statement
    {
        char const *const STATEMENT = R"x(
        if (1.1 == 1) {
            if (3 == 3) {
                11
            } else {
                22
            }
        } else {
            44
        }
        if (1.11 == 1) {
            if (3 == 3) {
                12314
            } else {
                22
            }
        } else {
            33
        }
        )x";
        std::cerr << parse::parse(STATEMENT) << std::endl;
    }
    // Invalid statement
    {
        char const *const STATEMENT = R"x(
        if (((((((((1.1)))) == (1.1)))))) {
            (((((22)))))
        } else {
            (44)
        })x";
        std::cerr << parse::parse(STATEMENT) << std::endl;
    }
    // Valid statement
    {
        char const *const STATEMENT = R"x(
        if (((((((((1.1.1)))) == (1.1)))))) {
            (((((22)))))
        } else {
            (44)
        })x";
        std::cerr << parse::parse(STATEMENT) << std::endl;
    }

    return 0;
}
