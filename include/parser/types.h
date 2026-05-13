#pragma once

#include "input.h"
#include "result.h"
#include "stack.h"

namespace parse {

    /*!
     * \brief A type that represents a comparison operation.
     */
    struct Compare {};

    /*!
     * \brief A type that represents a condition in an if statement.
     */
    struct Condition {};

    /*!
     * \brief A type that represents a block of code enclosed by curly braces.
     */
    struct ClosedBlock {};

    /*!
     * \brief A type that represents a block of code without curly braces.
     */
    struct Block {};

    /*!
     * \brief A type that represents an if statement.
     */
    struct If {};

    /*!
     * \brief Parse a given segment T from the input and return a parse result.
     */
    template <typename T>
    struct Type {
        static auto parse(Stack &stack, Input &input) -> parse::Result;
    };

} // namespace parse
