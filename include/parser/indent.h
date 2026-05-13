#pragma once

#include "input.h"

#include <iostream>
#include <string>
#include <thread>

namespace parse {

/*!
 * \brief A helper class to print indented debug messages for function entry
 * and exit.
 */
class IndentLogger {
public:
  /*!
   * \brief Constructs an indent object that prints an entry message with the
   * function name and input.
   *
   * \param input The current input being processed, used for debugging
   * purposes.
   * \param func_name The name of the function being entered, used for
   * debugging purposes.
   */
  IndentLogger(::parse::Input const &input, std::string_view func_name);

  /*!
   * \brief Destructs the indent object and prints an exit message with the
   * function name and
   */
  ~IndentLogger();

  /*!
   * \brief Overloads the stream insertion operator to print a debug message
   * with the current indentation level.
   *
   * \tparam T The type of the value being printed.
   * \param value The value to be printed.
   * \return A reference to the current IndentLogger object, allowing for
   * chaining of stream insertion operations.
   */
  template <typename T> auto operator<<(T const &value) -> IndentLogger & {
    auto &data = indents(std::this_thread::get_id());
    std::cerr << std::string(data.level * 2, ' ') << value << std::endl;
    return *this;
  }

private:
  /*! \brief The input being processed, used for debugging purposes. */
  ::parse::Input const &_input;
  /*! \brief The name of the function being entered, used for debugging
   * purposes. */
  std::string_view _func_name;

  /*! \brief A struct to hold the function name and indentation level for each
   * thread. */
  struct Data {
    char const *func = nullptr;
    size_t level = 0;
  };

  /*! \brief A static method to retrieve the Data struct for a given thread ID.
   */
  static auto indents(std::thread::id id) -> Data &;
};

} // namespace parse

#ifdef DEBUG
#define DEBUG_INDENT                                                           \
  IndentLogger _indentLogger##__COUNTER__(_input, __PRETTY_FUNCTION__)
#else
#define DEBUG_INDENT
#endif
