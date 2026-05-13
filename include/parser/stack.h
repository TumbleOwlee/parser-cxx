#pragma once

#include "layer.h"

#include <stack>
#include <variant>

namespace parse {

/*!
 * \brief Parser stack for storing layers during parsing.
 *
 * \details The Stack class manages a stack of Layer objects during the parsing
 * process. It provides methods to push and pop layers, as well as to evaluate
 * the current state of the stack. The stack is implemented using std::stack,
 * and it allows for type-safe popping of values from the top layer.
 */
struct Stack {
public:
  /*!
   * \brief Pops a value of type T from the top layer of the stack.
   * \tparam T The type of value to pop from the top layer.
   * \return The value of type T popped from the top layer.
   */
  template <typename T> inline auto pop() -> T {
    T value = *std::get_if<T>(&_stack.top());
    _stack.pop();
    return value;
  }

  /*!
   * \brief Pops the top layer from the stack.
   * \return The Layer object that was popped from the stack.
   */
  auto pop_layer() -> Layer;

  /*!
   * \brief Retrieves the top layer of the stack without modifying it.
   * \return A constant reference to the top Layer object on the stack.
   */
  auto top() const -> ::parse::Layer const &;

  /*!
   * \brief Retrieves the top layer of the stack.
   * \return A reference to the top Layer object on the stack.
   */
  auto top() -> ::parse::Layer &;

  /*!
   * \brief Pushes a new layer onto the stack.
   * \param layer The Layer object to be pushed onto the stack.
   */
  auto push(::parse::Layer &&layer) -> void;

  /*!
   * \brief Evaluates the current state of the stack.
   * \return A boolean value indicating the result of the evaluation.
   */
  auto eval() -> bool;

  /*!
   * \brief Sets the evaluation state of the stack.
   * \param value A boolean value to set the evaluation state of the stack.
   */
  auto eval(bool value) -> void;

private:
  /*! \brief A boolean flag indicating whether evaluation is enabled or
   * temporarily disabled. */
  bool _eval = true;
  /*! \brief The stack of Layer objects used during parsing. */
  std::stack<::parse::Layer> _stack;
};

} // namespace parse
