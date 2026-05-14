#pragma once

#include "input.h"

#include <functional>
#include <string>

namespace parse {

    /*!
     * \brief Skip whitespace characters in the input.
     * \param input The input to skip whitespace from.
     */
    inline auto skip(::parse::Input &input) -> void {
        while (*input != '\0' && (*input == ' ' || *input == '\n' || *input == '\t')) {
            ++input;
        }
    }

    inline auto peak(::parse::Input &input, char const *match) -> bool {
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
        return true;
    }

    /*!
     * \brief Match a specific string in the input.
     * \param input The input to match against.
     * \param match The string to match.
     * \return True if the match is successful, false otherwise.
     */
    inline auto match(::parse::Input &input, char const *match) -> bool {
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

    /*!
     * \brief Take characters from the input while a condition is not met.
     * \param input The input to take characters from.
     * \param stop_when A function that returns true when the stopping condition is
     * met.
     * \return A string containing the taken characters.
     */
    inline auto take_while(::parse::Input &input, std::function<bool(char)> stop_when) -> std::string {
        auto _input = input;
        while (*_input != '\0' && !stop_when(*_input)) {
            ++_input;
        }
        std::string res(input.data(), _input.data() - input.data());
        skip(_input);
        input = _input;
        return res;
    }

} // namespace parse
