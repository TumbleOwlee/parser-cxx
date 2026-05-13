#pragma once

#include <cstddef>

namespace parse {

/*!
 * \brief A class representing the input being parsed, providing methods for
 *
 * \details This class encapsulates a pointer to the original input string and a
 * pointer to the current position in the input. It provides methods for
 * accessing the current input, advancing the input pointer, and comparing input
 * positions. This class is designed to facilitate parsing operations by
 * allowing easy manipulation of the input string while keeping track of the
 * original input for error reporting purposes.
 */
struct Input {
public:
  /*!
   * \brief Constructs an input object with the given input string.
   *
   * \param ptr A pointer to the input string to be parsed. The input object
   * will keep track of the original input string and the current position in
   * the input. The current position is initialized to the beginning of the
   * input string.
   */
  Input(char const *ptr);

  /*!
   * \brief Get the current input string being parsed.
   *
   * \return A pointer to the current position in the input string. This pointer
   * can be used to access the remaining input that has not yet been parsed.
   */
  auto data() const -> char const *;

  /*!
   * \brief Get the original input string that was provided to the input object.
   *
   * \return A pointer to the original input string. This pointer can be used
   * for error reporting and debugging purposes, as it allows access to the
   * entire input string regardless of the current parsing position.
   */
  auto orig() const -> char const *;

  /*!
   * \brief Advances the input pointer to the next character in the input
   * string.
   *
   * \return A reference to the input object itself, allowing for method
   * chaining and continued parsing operations. This operator is used to move
   * the current position in the input string forward by one character,
   * facilitating the parsing process.
   */
  auto operator++() -> Input &;

  /*!
   * \brief Advances the input pointer by a specified number of characters in
   * the input string.
   *
   * \param offset The number of characters to advance the input pointer. The
   * input pointer will be moved forward by this number of characters, allowing
   * for skipping over sections of the input string during parsing.
   *
   * \return A reference to the input object itself, allowing for method
   * chaining and continued parsing operations. This operator is used to move
   * the current position in the input string forward by a specified number of
   * characters, facilitating more flexible parsing strategies.
   */
  auto operator+=(size_t offset) -> Input &;

  /*!
   * \brief Assignment operator for the input class, allowing one input object
   * to be assigned the value of another.
   *
   * \param other The input object to be assigned to this input object. The
   * current position in the input string will be updated to match that of the
   * other input object, while the original input string remains unchanged.
   *
   * \return A reference to the input object itself, allowing for method
   * chaining and continued parsing operations. This operator is used to update
   * the current position in the input string based on another input object,
   * facilitating flexible parsing strategies and error handling.
   */
  auto operator=(Input const &other) -> Input &;

  /*!
   * \brief Addition operator for the input class, allowing the input pointer to
   * be advanced by a specified number of characters.
   *
   * \param offset The number of characters to advance the input pointer. The
   * input pointer will be moved forward by this number of characters, allowing
   * for skipping over sections of the input string during parsing.
   *
   * \return A pointer to the new position in the input string after advancing
   * by the specified offset. This operator is used to calculate a new position
   * in the input string based on the current position and a specified offset,
   * facilitating flexible parsing strategies and error handling.
   */
  auto operator+(size_t offset) const -> char const *;

  /*!
   * \brief Subtraction operator for the input class, allowing the input pointer
   * to be moved backward by a specified number of characters.
   *
   * \param offset The number of characters to move the input pointer backward.
   * The input pointer will be moved backward by this number of characters,
   * allowing for backtracking during parsing operations.
   *
   * \return A pointer to the new position in the input string after moving
   * backward by the specified offset. This operator is used to calculate a new
   * position in the input string based on the current position and a specified
   * offset, facilitating flexible parsing strategies and error handling.
   */
  auto operator-(size_t offset) const -> char const *;

  /*!
   * \brief Dereference operator for the input class, allowing access to the
   * current character in the input string.
   *
   * \return The current character at the input pointer. This operator is used
   * to access the character currently being parsed, facilitating the parsing
   * process by allowing examination of the input string at the current
   * position.
   */
  auto operator*() const -> char;

  /*!
   * \brief Post-increment operator for the input class, allowing the input
   * pointer to be advanced to the next character in the input string.
   *
   * \return A reference to the input object itself, allowing for method
   * chaining and continued parsing operations. This operator is used to move
   * the current position in the input string forward by one character,
   * facilitating the parsing process.
   */
  auto operator++(int) -> Input &;

  /*!
   * \brief Equality operator for the input class, allowing comparison of two
   * input objects based on their current positions in the input string.
   *
   * \param other The input object to compare with this input object. The
   * comparison is based on the current position of the input pointer in the
   * input string.
   *
   * \return True if the current positions of both input objects are the same,
   * indicating that they are at the same point in the input string; otherwise,
   * false. This operator is used to compare two input objects during parsing
   * operations, facilitating error handling and backtracking strategies.
   */
  auto operator==(Input const &other) const -> bool;

  /*!
   * \brief Inequality operator for the input class, allowing comparison of two
   * input objects based on their current positions in the input string.
   *
   * \param other The input object to compare with this input object. The
   * comparison is based on the current position of the input pointer in the
   * input string.
   *
   * \return True if the current positions of both input objects are different,
   * indicating that they are at different points in the input string;
   * otherwise, false. This operator is used to compare two input objects during
   * parsing operations, facilitating error handling and backtracking
   * strategies.
   */
  auto operator!=(Input const &other) const -> bool;

private:
  char const *const _orig;
  char const *_ptr;
};

} // namespace parse
