#pragma once

#include <iostream>
#include <variant>

namespace parse {

    /*!
     * \brief A class that can hold a value of type int, double, string, or bool.
     *
     * This class is used to represent the value of a variable in the interpreter.
     * It can hold a value of type int, double, string, or bool. The class also
     * provides a constructor that takes a value of any of these types and
     * initializes the variant accordingly. Additionally, it provides an operator
     * bool() that returns true if the contained value is mappable to a boolean
     * value true (e.g., non-zero for int, non-empty for string, etc.) and false
     * otherwise.
     */
    class Value : public std::variant<std::monostate, int, double, std::string, bool> {
      public:
        /*!
         * \brief Constructs a Value from a given value of type T.
         *
         * This constructor allows for implicit conversion from types int, double,
         * std::string, and bool to the Value class. It initializes the underlying
         * std::variant with the provided value.
         *
         * \tparam T The type of the value being passed. Must be one of int, double,
         * std::string, or bool.
         * \param value The value to be stored in the Value object.
         */
        template <typename T>
        Value(T value) : std::variant<std::monostate, int, double, std::string, bool>(value) {}

        /*!
         * \brief Default constructor for Value.
         *
         * This constructor initializes the Value object with a default state, which
         * is represented by std::monostate. This means that the Value object will
         * not hold any meaningful value until it is assigned one of the supported
         * types (int, double, std::string, or bool).
         */
        Value();

        /*!
         * \brief Converts the contained value to a boolean representation.
         *
         * This operator allows for implicit conversion of a Value object to a bool.
         * The conversion is based on the type of the contained value:
         * - For std::string: returns true if the string is non-empty, false
         * otherwise.
         * - For double: returns true if the value is non-zero, false otherwise.
         * - For int: returns true if the value is non-zero, false otherwise.
         * - For bool: returns the contained boolean value directly.
         * - For std::monostate: returns false.
         *
         * \return A boolean representation of the contained value based on the rules
         * described above.
         */
        operator bool();
    };

    /*!
     * \brief Overloads the stream insertion operator for the Value class.
     *
     * This function allows for easy printing of Value objects to output streams
     * (e.g., std::cout). It checks the type of the contained value and formats
     * the output accordingly:
     * - For std::string: prints "String { value }".
     * - For double: prints "Double { value }".
     * - For int: prints "Integer { value }".
     * - For bool: prints "Bool { value }".
     *
     * \param out The output stream to which the Value object will be printed.
     * \param value The Value object to be printed.
     * \return A reference to the output stream after printing the Value object.
     */
    auto operator<<(std::ostream &out, Value const &value) -> std::ostream &;

} // namespace parse
