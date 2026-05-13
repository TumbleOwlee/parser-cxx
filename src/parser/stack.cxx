#include "stack.h"

auto ::parse::Stack::pop_layer() -> Layer {
  ::parse::Layer value = std::move(_stack.top());
  _stack.pop();
  std::cerr << "Pop Stack:  " << _stack.size() << std::endl;
  return value;
}

auto ::parse::Stack::top() const -> ::parse::Layer const & {
  return _stack.top();
}

auto ::parse::Stack::top() -> ::parse::Layer & { return _stack.top(); }

auto ::parse::Stack::push(::parse::Layer &&layer) -> void {
  _stack.push(layer);
  std::cerr << "Push Stack: " << _stack.size() << std::endl;
}

auto ::parse::Stack::eval() -> bool { return _eval; }

auto ::parse::Stack::eval(bool value) -> void { _eval = value; }
