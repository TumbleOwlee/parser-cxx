#pragma once

#include "input.h"

#include <iostream>

namespace parse {

    /*!
     * \brief Parser error holding context information for error reporting.
     */
    class Error {
      public:
        /*!
         * \brief Construct a new Error object.
         *
         * \param input The input context where the error occurred.
         * \param len The length of the error span in the input.
         */
        Error(::parse::Input const &input, size_t len);

        /*!
         * \brief Copy constructor for Error.
         *
         * \param other The Error object to copy from.
         */
        Error(Error const &other);

        /*!
         * \brief Move constructor for Error.
         *
         * \param other The Error object to move from.
         */
        Error(Error &&other);

        /*!
         * \brief Move assignment operator for Error.
         *
         * \param other The Error object to move from.
         * \return A reference to the assigned Error object.
         */
        auto operator=(Error &&other) -> Error &;

        /*!
         * \brief Comparison operator for Error objects.
         *
         * \param other The Error object to compare with.
         * \return true if this error position is less than the other error position,
         * or if equal then compare lengths; false otherwise.
         */
        auto operator<(Error const &other) const -> bool;

        /*!
         * \brief Get the length of the error span.
         *
         * \return The length of the error span.
         */
        auto len() const -> size_t;

        /*!
         * \brief Get the input context of the error.
         *
         * \return A reference to the input context where the error occurred.
         */
        auto input() const -> ::parse::Input const &;

      private:
        /*! \brief The length of the error span in the input. */
        size_t _len;
        /*! \brief The input context where the error occurred. */
        ::parse::Input _input;
    };

    /*!
     * \brief Output stream operator for Error objects.
     *
     * \param out The output stream to write to.
     * \param error The Error object to output.
     * \return A reference to the output stream after writing the error information.
     */
    auto operator<<(std::ostream &out, ::parse::Error const &error) -> std::ostream &;

} // namespace parse
