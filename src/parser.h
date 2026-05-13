#include <functional>
#include <iostream>
#include <optional>
#include <stack>
#include <string>
#include <variant>

struct input {
public:
  input(char const *ptr) : _orig(ptr), _ptr(ptr) {}

  auto data() const -> char const * { return _ptr; }

  auto orig() const -> char const * { return _orig; }

  auto operator++() -> input & {
    ++_ptr;
    return *this;
  }

  auto operator+=(size_t offset) -> input & {
    _ptr += offset;
    return *this;
  }

  auto operator=(input const &other) -> input & {
    _ptr = other._ptr;
    return *this;
  }

  auto operator+(size_t offset) const -> char const * { return _ptr + offset; }

  auto operator-(size_t offset) const -> char const * { return _ptr - offset; }

  auto operator*() const -> char { return *_ptr; }

  auto operator++(int) -> input {
    input tmp = *this;
    ++_ptr;
    return tmp;
  }

  auto operator==(input const &other) const -> bool {
    return _ptr == other._ptr;
  }

  auto operator!=(input const &other) const -> bool {
    return _ptr != other._ptr;
  }

private:
  char const *const _orig;
  char const *_ptr;
};

struct parse_error {
public:
  parse_error(input const &input) : _input(input) {}

  friend auto operator<<(std::ostream &out, parse_error const &error)
      -> std::ostream & {
    static constexpr size_t context_size = 20;

    size_t offset = (error._input.data() - error._input.orig());
    if (offset > context_size) {
      offset = context_size;
    }

    size_t end = 0;
    while (*(error._input.data() + end) != '\0' && end <= context_size) {
      ++end;
    }

    out << "\033[1;31mERROR:\033[0m  "
        << std::string_view(error._input.data() - offset, offset + end)
        << (end > context_size ? "..." : "") << std::endl;
    out << "        " << std::string(offset, ' ')
        << "\033[1;31m^ Unexpected token.\033[0m" << std::endl;

    return out;
  }

  parse_error(parse_error &&other) : _input(other._input) {}

  auto operator=(parse_error &&other) -> parse_error & {
    _input = other._input;
    return *this;
  }

private:
  ::input _input;
};

struct result_type {
public:
  result_type() {}

  result_type(input const input) : _error(input) {}

  result_type(result_type &&other) : _error(std::move(other._error)) {}

  auto operator=(result_type &&other) -> result_type & {
    _error = std::move(other._error);
    return *this;
  }

  operator bool() { return !_error.has_value(); }

  friend auto operator<<(std::ostream &out, result_type const &result)
      -> std::ostream & {
    if (result._error) {
      out << result._error.value();
    } else {
      out << " \033[1;32mParse successful.\033[0m" << std::endl;
    }
    return out;
  }

private:
  std::optional<parse_error> _error;
};

using value_type = std::variant<std::monostate, int, double, std::string>;

inline void print_value_type(value_type const &value) {
  if (auto v = std::get_if<std::string>(&value)) {
    std::cerr << "String { " << *v << " }" << std::endl;
  } else if (auto v = std::get_if<double>(&value)) {
    std::cerr << "Double { " << *v << " }" << std::endl;
  } else if (auto v = std::get_if<int>(&value)) {
    std::cerr << "Integer { " << *v << " }" << std::endl;
  }
}

using return_value = result_type;

enum compare_operator {
  less,
  less_equal,
  equal,
  not_equal,
  more,
  more_equal,
};

template <typename T, typename S> struct comparer {
  static auto compare(T const &left, S const &right, compare_operator op)
      -> bool {
    switch (op) {
    case compare_operator::less:
      return left < right;
    case compare_operator::less_equal:
      return left <= right;
    case compare_operator::equal:
      return left == right;
    case compare_operator::not_equal:
      return left != right;
    case compare_operator::more:
      return left > right;
    case compare_operator::more_equal:
      return left >= right;
    defaut:
      return false;
    }
    return false;
  }
};

inline auto apply_compare(value_type const &left, value_type const &right,
                          compare_operator op) -> bool {

#define apply_compare_type(T, S)                                               \
  do {                                                                         \
    {                                                                          \
      T const *v1 = std::get_if<T>(&left);                                     \
      S const *v2 = std::get_if<S>(&right);                                    \
      if (v1 != nullptr && v2 != nullptr) {                                    \
        return comparer<T, S>::compare(*v1, *v2, op);                          \
      }                                                                        \
    }                                                                          \
    {                                                                          \
      S const *v1 = std::get_if<S>(&left);                                     \
      T const *v2 = std::get_if<T>(&right);                                    \
      if (v1 != nullptr && v2 != nullptr) {                                    \
        return comparer<S, T>::compare(*v1, *v2, op);                          \
      }                                                                        \
    }                                                                          \
  } while (false)

  apply_compare_type(int, int);
  apply_compare_type(int, double);
  apply_compare_type(double, double);
  apply_compare_type(std::string, std::string);

#undef apply_compare_type

  return false;
}

struct condition {
  bool result;
};

template <typename T> using opt = std::optional<T>;

using layer =
    std::variant<std::monostate, condition, value_type, compare_operator>;

using stack = std::stack<layer>;

template <typename T> inline auto pop_stack(stack &stack) -> T {
  T value = *std::get_if<T>(&stack.top());
  stack.pop();
  return value;
}

inline auto pop_stack(stack &stack, size_t num) -> void {
  for (auto i = 0U; i < num; ++i) {
    stack.pop();
  }
}

inline auto skip(::input &input) -> void {
  while (*input != '\0' &&
         (*input == ' ' || *input == '\n' || *input == '\t')) {
    ++input;
  }
}

inline auto match(::input &input, ::input match) -> bool {
  auto _input = input;
  while (*match != '\0') {
    if (*_input == '\0') {
      return false;
    }

    if (*_input == *match) {
      ++_input;
      ++match;
    } else {
      return false;
    }
  }

  skip(_input);
  input = _input;
  return true;
}

inline auto take_while(::input &input, std::function<bool(char)> stop_when)
    -> std::string {
  auto _input = input;
  while (!stop_when(*_input)) {
    ++_input;
  }
  std::string res(input.data(), _input.data() - input.data());
  skip(_input);
  input = _input;
  return res;
}

inline auto parse_float(stack &stack, input &input) -> return_value {
  auto _input = input;

  std::string value =
      take_while(_input, [](char c) { return c < '0' || c > '9'; });
  if (value.empty()) {
    return _input;
  }
  if (!match(_input, ".")) {
    return _input;
  }
  std::string fraction =
      take_while(_input, [](char c) { return c < '0' || c > '9'; });

  std::string num = value + "." + fraction;
  value_type vtype = std::atof(num.c_str());

  stack.push(vtype);
  input = _input;
  return {};
}

inline auto parse_integer(stack &stack, input &input) -> return_value {
  auto _input = input;

  std::string value =
      take_while(_input, [](char c) { return c < '0' || c > '9'; });
  if (value.empty()) {
    return _input;
  }

  value_type vtype = std::atoi(value.c_str());

  stack.push(vtype);
  input = _input;
  return {};
}

inline auto parse_string(stack &stack, input &input) -> return_value {
  auto _input = input;

  if (!match(_input, "\"")) {
    return _input;
  }
  value_type value = take_while(_input, [](char c) { return c == '"'; });
  if (!match(_input, "\"")) {
    return _input;
  }

  stack.push(value);
  input = _input;
  return {};
}

inline auto parse_value(stack &stack, input &input) -> return_value {
  auto _input = input;
  return_value return_value;
  if ((return_value = parse_string(stack, _input))) {
    input = _input;
    return {};
  } else if ((return_value = parse_float(stack, _input))) {
    input = _input;
    return {};
  } else if ((return_value = parse_integer(stack, _input))) {
    input = _input;
    return {};
  }
  return return_value;
}

inline auto parse_operator(stack &stack, input &input) -> return_value {
  auto _input = input;

  if (match(_input, "==")) {
    stack.push(compare_operator::equal);
  } else if (match(_input, "!=")) {
    stack.push(compare_operator::not_equal);
  } else if (match(_input, ">=")) {
    stack.push(compare_operator::more_equal);
  } else if (match(_input, "<=")) {
    stack.push(compare_operator::less_equal);
  } else if (match(_input, "<")) {
    stack.push(compare_operator::less);
  } else if (match(_input, ">")) {
    stack.push(compare_operator::more);
  } else {
    return _input;
  }

  input = _input;
  return {};
}

inline auto parse_compare(stack &stack, input &input) -> return_value {
  auto _input = input;
  return_value return_value;

  if (!(return_value = parse_value(stack, _input))) {
    return return_value;
  }
  if (!(return_value = parse_operator(stack, _input))) {
    pop_stack(stack, 1);
    return return_value;
  }
  if (!(return_value = parse_value(stack, _input))) {
    pop_stack(stack, 2);
    return return_value;
  }

  auto right = pop_stack<value_type>(stack);
  auto op = pop_stack<compare_operator>(stack);
  auto left = pop_stack<value_type>(stack);

  stack.push(condition{.result = apply_compare(left, right, op)});

  input = _input;
  return {};
}

inline auto parse_condition(stack &stack, input &input) -> return_value {
  auto _input = input;
  return_value return_value;

  if (match(_input, "(")) {
    if (!(return_value = parse_condition(stack, _input))) {
      return return_value;
    }
    if (!match(_input, ")")) {
      pop_stack(stack, 1);
      return _input;
    }
  } else {
    if (!(return_value = parse_compare(stack, _input))) {
      return return_value;
    }
  }

  if (match(_input, "&&")) {
    if (!parse_condition(stack, _input)) {
      pop_stack(stack, 1);
      return _input;
    }

    auto if_left = pop_stack<::condition>(stack);
    auto if_right = pop_stack<::condition>(stack);

    stack.push(condition{.result = if_left.result && if_right.result});
  } else if (match(_input, "||")) {
    if (!(return_value = parse_condition(stack, _input))) {
      pop_stack(stack, 1);
      return return_value;
    }

    auto if_left = pop_stack<::condition>(stack);
    auto if_right = pop_stack<::condition>(stack);

    stack.push(condition{.result = if_left.result || if_right.result});
  }

  input = _input;
  return {};
}

inline auto parse_if(stack &stack, input &input) -> return_value;

inline auto parse_block(stack &stack, input &input) -> return_value {
  auto _input = input;

  if (!match(_input, "{")) {
    return _input;
  }

  if (parse_if(stack, _input)) {
  } else if (parse_value(stack, _input)) {
  } else {
    return _input;
  }

  if (!match(_input, "}")) {
    pop_stack(stack, 1);
    return _input;
  }

  input = _input;
  return {};
}

inline auto parse_if(stack &stack, input &input) -> return_value {
  bool with_else = false;
  auto _input = input;
  return_value return_value;

  if (!match(_input, "if")) {
    return _input;
  }
  if (!(return_value = parse_condition(stack, _input))) {
    return return_value;
  }
  if (!(return_value = parse_block(stack, _input))) {
    pop_stack(stack, 1);
    return return_value;
  }
  if (match(_input, "else")) {
    with_else = true;
    if (!(return_value = parse_block(stack, _input))) {
      pop_stack(stack, 2);
      return return_value;
    }
  }

  ::value_type else_result;
  if (with_else) {
    else_result = pop_stack<::value_type>(stack);
  }

  auto if_result = pop_stack<::value_type>(stack);
  auto if_condition = pop_stack<::condition>(stack);

  if (if_condition.result) {
    stack.push(if_result);
  } else {
    stack.push(else_result);
  }

  input = _input;
  return {};
}

inline auto parse(input input) -> return_value {
  stack stack;
  return_value return_value;

  skip(input);

  if (!(return_value = parse_if(stack, input))) {
    return return_value;
  }

  auto value = pop_stack<::value_type>(stack);
  print_value_type(value);
  return {};
}
