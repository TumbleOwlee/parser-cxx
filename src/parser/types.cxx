#include "parser/types.h"
#include "parser/indent.h"
#include "parser/input.h"
#include "parser/result.h"
#include "parser/stack.h"
#include "parser/util.h"

#include <string>

// ============================================================
// Type: float
// ============================================================

template <>
auto ::parse::Type<float>::parse(Stack &stack, Input &input) -> ::parse::Result {
    auto _input = input;
    auto mult = 1.0;
    DEBUG_INDENT;

    if (match(_input, "-")) {
        mult = -1.0;
    }

    std::string value = take_while(_input, [](char c) { return c < '0' || c > '9'; });
    if (value.empty()) {
        return {_input, 1};
    }

    if (!match(_input, ".")) {
        return {_input - value.length(), value.length() + 1};
    }

    std::string fraction = take_while(_input, [](char c) { return c < '0' || c > '9'; });

    std::string num = value + "." + fraction;
    ::parse::Value vtype = mult * std::atof(num.c_str());

    stack.push(vtype);
    skip(_input);
    input = _input;
    return {};
}

// ============================================================
// Type: int
// ============================================================

template <>
auto ::parse::Type<int>::parse(Stack &stack, Input &input) -> ::parse::Result {
    auto _input = input;
    DEBUG_INDENT;
    int mult = 1;

    if (match(_input, "-")) {
        mult = -1;
    }

    std::string value = take_while(_input, [](char c) { return c < '0' || c > '9'; });
    if (value.empty()) {
        return {_input, 1};
    }

    ::parse::Value vtype = mult * std::atoi(value.c_str());

    stack.push(vtype);
    skip(_input);
    input = _input;
    return {};
}

// ============================================================
// Type: std::string
// ============================================================

template <>
auto ::parse::Type<std::string>::parse(Stack &stack, Input &input) -> ::parse::Result {
    auto _input = input;
    DEBUG_INDENT;

    if (!match(_input, "\"")) {
        return {_input, 1};
    }
    std::string value = take_while(_input, [](char c) { return c == '"'; });
    if (!match(_input, "\"")) {
        return {_input - 1 - value.length(), value.length() + 2};
    }

    stack.push(value);
    skip(_input);
    input = _input;
    return {};
}

// ============================================================
// Type: bool
// ============================================================

template <>
auto ::parse::Type<bool>::parse(Stack &stack, Input &input) -> ::parse::Result {
    auto _input = input;
    DEBUG_INDENT;

    if (match(_input, "true")) {
        stack.push(true);
    } else if (match(_input, "false")) {
        stack.push(false);
    } else {
        return {_input, 4};
    }
    skip(_input);
    input = _input;
    return {};
}

// ============================================================
// Type: ::parse::Value
// ============================================================

template <>
auto ::parse::Type<::parse::Value>::parse(Stack &stack, Input &input) -> ::parse::Result {
    auto _input = input;
    DEBUG_INDENT;
    ::parse::Result result;

    bool brackets = false;

    if (match(_input, "(")) {
        brackets = true;
    }

    if (peak(_input, "(")) {
        auto result = ::parse::Type<::parse::Value>::parse(stack, _input);
        if (!result) {
            return result;
        }
        goto parse_type_value_parse_exit;
    }

#define _PARSE_CHECK_TYPE(T)                                                                                           \
    {                                                                                                                  \
        ::parse::Result value;                                                                                         \
        if ((value = ::parse::Type<T>::parse(stack, _input))) {                                                        \
            goto parse_type_value_parse_exit;                                                                          \
        } else if (result || result.error() < value.error()) {                                                         \
            result = std::move(value);                                                                                 \
        }                                                                                                              \
    }

    _PARSE_CHECK_TYPE(std::string);
    _PARSE_CHECK_TYPE(float);
    _PARSE_CHECK_TYPE(int);
    _PARSE_CHECK_TYPE(bool);

#undef _PARSE_CHECK_TYPE
    return result;

parse_type_value_parse_exit:
    if (brackets && !match(_input, ")")) {
        return {_input, 1};
    }
    input = _input;
    return {};
}

// ============================================================
// Type: ::parse::compare::Comparison
// ============================================================

template <>
auto ::parse::Type<::parse::compare::Comparison>::parse(Stack &stack, Input &input) -> parse::Result {
    auto _input = input;
    DEBUG_INDENT;

    if (match(_input, "==")) {
        stack.push(parse::compare::Comparison::Equal);
    } else if (match(_input, "!=")) {
        stack.push(parse::compare::Comparison::NotEqual);
    } else if (match(_input, ">=")) {
        stack.push(parse::compare::Comparison::GreaterThan);
    } else if (match(_input, "<=")) {
        stack.push(parse::compare::Comparison::LessEqual);
    } else if (match(_input, "<")) {
        stack.push(parse::compare::Comparison::Less);
    } else if (match(_input, ">")) {
        stack.push(parse::compare::Comparison::Greater);
    } else {
        return {_input, 1};
    }

    input = _input;
    return {};
}

// ============================================================
// Type: ::parse::Compare
// ============================================================

template <>
auto ::parse::Type<::parse::Compare>::parse(Stack &stack, Input &input) -> ::parse::Result {
    auto _input = input;
    DEBUG_INDENT;

    auto brackets = false;
    ::parse::Value left_value;
    ::parse::Value right_value;
    ::parse::compare::Comparison comparison;

    ::parse::Result result = {nullptr, 0};

    if (match(_input, "(")) {
        brackets = true;
    }

    // Recursive by simple brackets
    if (peak(_input, "(")) {
        auto _input1 = _input;
        result = ::parse::Type<::parse::Compare>::parse(stack, _input1);
        if (result) {
            _input = _input1;
            goto parse_type_compare_parse_exit;
        }
    }

    // Recursive with left side in brackets
    if (peak(_input, "(")) {
        if (!(result = ::parse::Type<::parse::Value>::parse(stack, _input))) {
            return result;
        }
    } else if (!(result = Type<::parse::Value>::parse(stack, _input))) {
        return result;
    }
    left_value = stack.pop<::parse::Value>();

    if (!(result = ::parse::Type<::parse::compare::Comparison>::parse(stack, _input))) {
        return result;
    }
    comparison = stack.pop<parse::compare::Comparison>();

    if (!(result = Type<::parse::Value>::parse(stack, _input))) {
        return result;
    }
    right_value = stack.pop<::parse::Value>();

    stack.push(::parse::Value(::parse::compare::eval(left_value, comparison, right_value)));

parse_type_compare_parse_exit:
    if (brackets && !match(_input, ")")) {
        return {_input, 1};
    }

    input = _input;
    return {};
}

// ============================================================
// Type: ::parse::Condition
// ============================================================

template <>
auto ::parse::Type<::parse::Condition>::parse(Stack &stack, Input &input) -> ::parse::Result {
    auto _input = input;
    DEBUG_INDENT;

    parse::Result result(_input, 1);
    ::parse::Value left;

    if (!result) {
        auto _input1 = _input;
        if (match(_input1, "(")) {
            if ((result = ::parse::Type<::parse::Condition>::parse(stack, _input1))) {
                left = stack.pop<::parse::Value>();
                if (match(_input1, ")")) {
                    _input = _input1;
                } else {
                    result = {_input1, 1};
                }
            }
        }
    }

    if (!result) {
        auto _input1 = _input;
        if ((result = Type<::parse::Compare>::parse(stack, _input1))) {
            left = stack.pop<::parse::Value>();
            _input = _input1;
        }
    }

    if (!result) {
        auto _input1 = _input;
        if ((result = Type<::parse::Value>::parse(stack, _input1))) {
            left = stack.pop<::parse::Value>();
            _input = _input1;
        }
    }

    if (!result) {
        return result;
    }

    if (match(_input, "&&")) {
        if (!(result = ::parse::Type<::parse::Condition>::parse(stack, _input))) {
            return result;
        }

        auto right = stack.pop<::parse::Value>();
        stack.push(left && right);
    } else if (match(_input, "||")) {
        if (!(result = ::parse::Type<::parse::Condition>::parse(stack, _input))) {
            return result;
        }

        auto right = stack.pop<::parse::Value>();
        stack.push(left || right);
    } else {
        stack.push(left);
    }

    input = _input;
    return {};
}

// ============================================================
// Type: ::parse::If
// ============================================================

template <>
auto ::parse::Type<::parse::If>::parse(::parse::Stack &stack, ::parse::Input &input) -> ::parse::Result {
    auto _input = input;
    DEBUG_INDENT;

    bool with_else = false;
    parse::Result result;

    if (!match(_input, "if")) {
        return {_input, 2};
    }

    if (!match(_input, "(")) {
        return {_input, 1};
    }

    if (!(result = Type<::parse::Condition>::parse(stack, _input))) {
        return result;
    }
    auto condition = stack.pop<::parse::Value>();
    stack.eval(condition);

    if (!match(_input, ")")) {
        return {_input, 1};
    }

    if (!(result = Type<::parse::ClosedBlock>::parse(stack, _input))) {
        return result;
    }
    auto if_value = stack.pop<::parse::Value>();

    stack.eval(!condition);

    ::parse::Value else_value;
    if (match(_input, "else")) {
        if (!(result = Type<::parse::ClosedBlock>::parse(stack, _input))) {
            return result;
        }
        else_value = stack.pop<::parse::Value>();
    }

    if (condition) {
        stack.push(if_value);
    } else {
        stack.push(else_value);
    }

    input = _input;
    return {};
}
