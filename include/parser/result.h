#pragma once

#include "error.h"
#include "input.h"

#include <iostream>
#include <optional>

namespace parse {

/*!
 * \brief Result of a parse operation.
 */
struct Result {
public:
  /*!
   * \brief Construct a successful parse result.
   */
  Result();

  /*!
   * \brief Construct a failed parse result.
   *
   * \param input The input that caused the failure.
   * \param len The length of the input that caused the failure.
   */
  Result(Input const input, size_t const len);

  /*!
   * \brief Move constructor.
   *
   * \param other The result to move from.
   */
  Result(Result &&other);

  /*!
   * \brief Move assignment operator.
   *
   * \param other The result to move from.
   * \return A reference to this result.
   */
  auto operator=(Result &&other) -> Result &;

  /*!
   * \brief Check if the parse was successful.
   *
   * \return true if the parse was successful, false otherwise.
   */
  operator bool() const;

  /*!
   * \brief Get the error associated with a failed parse.
   *
   * \return The error associated with a failed parse.
   * \throws std::bad_optional_access if the parse was successful.
   */
  auto error() const -> parse::Error const &;

private:
  /*! \brief The error associated with a failed parse, if any. */
  std::optional<parse::Error> _error;
};

/*!
 * \brief Output a parse result to a stream.
 *
 * \param out The stream to output to.
 * \param result The result to output.
 * \return A reference to the output stream.
 */
auto operator<<(std::ostream &out, Result const &result) -> std::ostream &;

} // namespace parse
