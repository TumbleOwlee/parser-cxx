#include "parser/result.h"

::parse::Result::Result() {}

::parse::Result::Result(Input const input, size_t const len) : _error({input, len}) {}

::parse::Result::Result(::parse::Result &&other) : _error(std::move(other._error)) {}

auto ::parse::Result::operator=(::parse::Result && other) -> ::parse::Result & {
    _error = std::move(other._error);
    return *this;
}

::parse::Result::operator bool() const { return !_error.has_value(); }

auto ::parse::Result::error() const -> parse::Error const & { return _error.value(); }

auto ::parse::operator<<(std::ostream & out, ::parse::Result const &result) -> std::ostream & {
    if (!result) {
        out << result.error();
    } else {
        out << " \033[1;32mParse successful.\033[0m" << std::endl;
    }
    return out;
}
