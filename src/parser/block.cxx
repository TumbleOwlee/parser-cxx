#include "types.h"

#include "indent.h"
#include "input.h"
#include "result.h"
#include "stack.h"
#include "util.h"

// ============================================================
// Type: ::parse::Block
// ============================================================

template <>
auto ::parse::Type<::parse::Block>::parse(Stack &stack, Input &input)
    -> ::parse::Result {
  auto _input = input;
  DEBUG_INDENT;

  bool has_value = false;
  bool has_match = false;

  parse::Result result;

  while (*_input != '\0') {

    {
      ::parse::Result value;
      if ((value = Type<::parse::If>::parse(stack, _input))) {
        goto go_match;
      } else if (result || result.error() < value.error()) {
        result = std::move(value);
      }
    }

    {
      ::parse::Result value;
      if ((value = Type<::parse::Value>::parse(stack, _input))) {
        goto go_match;
      } else if (result || result.error() < value.error()) {
        result = std::move(value);
      }
    }

    return result;

  go_match:
    has_value = true;
    if (match(_input, ";")) {
      stack.pop_layer();
      has_value = false;
      break;
    }
  }

  if (!has_value) {
    stack.push(std::monostate{});
  }

  input = _input;
  return {};
}

// ============================================================
// Type: ::parse::ClosedBlock
// ============================================================

template <>
auto ::parse::Type<::parse::ClosedBlock>::parse(Stack &stack, Input &input)
    -> ::parse::Result {
  auto _input = input;
  DEBUG_INDENT;

  if (!match(_input, "{")) {
    return {_input, 1};
  }

  bool has_value = false;
  bool has_match = false;

  parse::Result result;

  while (*_input != '\0') {
    if ((has_match = match(_input, "}"))) {
      break;
    }

    {
      ::parse::Result value;
      if ((value = Type<::parse::If>::parse(stack, _input))) {
        goto go_match;
      } else if (result || result.error() < value.error()) {
        result = std::move(value);
      }
    }

    {
      ::parse::Result value;
      if ((value = Type<::parse::Value>::parse(stack, _input))) {
        goto go_match;
      } else if (result || result.error() < value.error()) {
        result = std::move(value);
      }
    }

    return result;

  go_match:
    has_value = true;
    if (match(_input, ";")) {
      stack.pop_layer();
      has_value = false;
      break;
    }
  }

  if (!has_value) {
    stack.push(std::monostate{});
  }

  if (!has_match && !match(_input, "}")) {
    stack.pop_layer();
    return {_input, 1};
  }

  input = _input;
  return {};
}
