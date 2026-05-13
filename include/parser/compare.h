#pragma once

#include "value.h"

/*!
 * \brief Defines the comparison operators and their evaluation logic for
 * comparing two ::parse::Value instances.
 *
 * This header declares the Comparison enum class, which represents the various
 * comparison operators (e.g., <, <=, ==, !=, >, >=). It also declares the eval
 * function, which takes two ::parse::Value instances and a Comparison operator,
 * and evaluates the comparison based on the types of the values.
 *
 * The eval function will handle comparisons between different types (e.g., int
 * vs double) and will return a boolean result indicating whether the comparison
 * holds true or not.
 */
namespace parse::compare {

    /*!
     * \brief An enumeration of comparison operators that can be used to compare
     * two ::parse::Value instances.
     *
     * The Comparison enum class includes the following operators:
     * - Less: Represents the '<' operator.
     * - LessEqual: Represents the '<=' operator.
     * - Equal: Represents the '==' operator.
     * - NotEqual: Represents the '!=' operator.
     * - Greater: Represents the '>' operator.
     * - GreaterThan: Represents the '>=' operator.
     */
    enum class Comparison {
        Less,
        LessEqual,
        Equal,
        NotEqual,
        Greater,
        GreaterThan,
    };

    /*!
     * \brief Evaluates the comparison between two ::parse::Value instances based on
     * the specified Comparison operator.
     *
     * This function takes two ::parse::Value instances (left and right) and a
     * Comparison operator, and evaluates the comparison according to the types of
     * the values. The function will handle comparisons between different types
     * (e.g., int vs double) and will return a boolean result indicating whether the
     * comparison holds true or not.
     *
     * \param left The left-hand side value to be compared.
     * \param comparison The Comparison operator that specifies the type of
     * comparison to be performed.
     * \param right The right-hand side value to be compared.
     * \return A boolean result indicating whether the comparison holds true or not.
     */
    auto eval(::parse::Value const &left, Comparison comparison, ::parse::Value const &right) -> bool;

} // namespace parse::compare
