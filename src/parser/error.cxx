#include "parser/error.h"

#include <iomanip>
#include <sstream>

::parse::Error::Error(::parse::Input const &input, size_t len) : _len(len), _input(input) {}

::parse::Error::Error(::parse::Error const &other) : _len(other._len), _input(other._input) {}

::parse::Error::Error(::parse::Error &&other) : _len(other._len), _input(other._input) {}

auto ::parse::Error::operator=(::parse::Error && other) -> Error & {
    _input = other._input;
    _len = other._len;
    return *this;
}

auto ::parse::Error::operator<(::parse::Error const &other) const -> bool {
    return _input.data() < other._input.data() || (_input.data() == other._input.data() && _len < other._len);
}

auto ::parse::Error::len() const -> size_t { return _len; }

auto ::parse::Error::input() const -> ::parse::Input const & { return _input; }

auto ::parse::operator<<(std::ostream & out, ::parse::Error const &error) -> std::ostream & {
    static constexpr size_t CONTEXT_LENGTH = 5 + 1;
    std::string lines[CONTEXT_LENGTH];
    size_t start = 0, end = 0, num = 1;
    std::optional<size_t> pending;

    auto orig = error.input().orig();
    auto data = error.input().data();
    char const *pos = orig;

    std::cerr << "\033[1;31mERROR:\033[0m" << std::endl << std::endl;
    while ((!pending || *pending > 0) && *orig != '\0') {
        while (*pos != '\0' && *pos != '\n') {
            ++pos;
        }
        std::stringstream buf;
        auto line = std::string_view(orig, pos - orig);
        buf << std::setw(4) << num << " | " << line << std::endl;
        ++num;

        if (orig <= data && data < pos) {
            buf << "   \033[1;31mE\033[0m | " << std::string(data - orig, ' ') << "\033[1;31m"
                << std::string(error.len(), '^') << " Unexpected token.\033[0m" << std::endl;
            pending = CONTEXT_LENGTH / 2;
        } else if (pending) {
            --(*pending);
        }

        lines[end] = buf.str();
        end = (end + 1) % CONTEXT_LENGTH;
        if (start == end) {
            start = (start + 1) % CONTEXT_LENGTH;
        }

        if (*pos != '\0') {
            ++pos;
        }
        orig = pos;
    }

    while (start != end) {
        std::cerr << lines[start];
        start = (start + 1) % CONTEXT_LENGTH;
    }

    return out;
}
