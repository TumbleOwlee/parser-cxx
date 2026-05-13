#include "types.h"
#include "indent.h"
#include "input.h"
#include "result.h"
#include "stack.h"
#include "util.h"

#include <string>

// ============================================================
// Type: float
// ============================================================

template <>
auto ::parse::Type<float>::parse(Stack &stack, Input &input)
    -> ::parse::Result {
  auto _input = input;
  auto mult = 1.0;
  DEBUG_INDENT;

  if (match(_input, "-")) {
    mult = -1.0;
  }

  std::string value =
      take_while(_input, [](char c) { return c < '0' || c > '9'; });
  if (value.empty()) {
    return {_input, 1};
  }

  if (!match(_input, ".")) {
    return {_input - value.length(), value.length() + 1};
  }

  std::string fraction =
      take_while(_input, [](char c) { return c < '0' || c > '9'; });

  std::string num = value + "." + fraction;
  ::parse::Value vtype = mult * std::atof(num.c_str());

  stack.push(vtype);
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

  std::string value =
      take_while(_input, [](char c) { return c < '0' || c > '9'; });
  if (value.empty()) {
    return {_input, 1};
  }

  ::parse::Value vtype = mult * std::atoi(value.c_str());

  stack.push(vtype);
  input = _input;
  return {};
}

// ============================================================
// Type: std::string
// ============================================================

template <>
auto ::parse::Type<std::string>::parse(Stack &stack, Input &input)
    -> ::parse::Result {
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
  input = _input;
  return {};
}

// ============================================================
// Type: ::parse::Value
// ============================================================

template <>
auto ::parse::Type<::parse::Value>::parse(Stack &stack, Input &input)
    -> ::parse::Result {
  auto _input = input;
  DEBUG_INDENT;
  ::parse::Result result;

#define _PARSE_CHECK_TYPE(T)                                                   \
  {                                                                            \
    ::parse::Result value;                                                     \
    if ((value = ::parse::Type<T>::parse(stack, _input))) {                    \
      input = _input;                                                          \
      return {};                                                               \
    } else if (result || result.error() < value.error()) {                     \
      result = std::move(value);                                               \
    }                                                                          \
  }

  _PARSE_CHECK_TYPE(std::string);
  _PARSE_CHECK_TYPE(float);
  _PARSE_CHECK_TYPE(int);
  _PARSE_CHECK_TYPE(bool);

#undef _PARSE_CHECK_TYPE

  return result;
}

// ============================================================
// Type: ::parse::compare::Comparison
// ============================================================

template <>
auto ::parse::Type<::parse::compare::Comparison>::parse(Stack &stack,
                                                        Input &input)
    -> parse::Result {
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
auto ::parse::Type<::parse::Compare>::parse(Stack &stack, Input &input)
    -> ::parse::Result {
  auto _input = input;
  DEBUG_INDENT;

  ::parse::Result result;

  if (!(result = Type<::parse::Value>::parse(stack, _input))) {
    return result;
  }
  auto left = stack.pop<::parse::Value>();

  if ((result =
           ::parse::Type<::parse::compare::Comparison>::parse(stack, _input))) {
    auto op = stack.pop<parse::compare::Comparison>();
    if (!(result = Type<::parse::Value>::parse(stack, _input))) {
      return result;
    }
    auto right = stack.pop<::parse::Value>();

    stack.push(::parse::Value(::parse::compare::eval(left, op, right)));
  } else {
    auto left = stack.pop<::parse::Value>();
    stack.push(::parse::Value(left));
  }

  input = _input;
  return {};
}

// ============================================================
// Type: ::parse::Condition
// ============================================================

template <>
auto ::parse::Type<::parse::Condition>::parse(Stack &stack, Input &input)
    -> ::parse::Result {
  auto _input = input;
  DEBUG_INDENT;

  parse::Result result;
  ::parse::Value left;

  if (match(_input, "(")) {
    if (!(result = ::parse::Type<::parse::Condition>::parse(stack, _input))) {
      return result;
    }
    left = stack.pop<::parse::Value>();
    if (!match(_input, ")")) {
      return {_input, 1};
    }
  } else {
    if (!(result = Type<::parse::Compare>::parse(stack, _input))) {
      return result;
    }
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
  }

  input = _input;
  return {};
}

// ============================================================
// Type: ::parse::If
// ============================================================

template <>
auto ::parse::Type<::parse::If>::parse(::parse::Stack &stack,
                                       ::parse::Input &input)
    -> ::parse::Result {
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
