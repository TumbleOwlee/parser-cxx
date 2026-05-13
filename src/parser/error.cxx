#include "parser/error.h"

::parse::Error::Error(::parse::Input const &input, size_t len)
    : _len(len), _input(input) {}

::parse::Error::Error(::parse::Error const &other)
    : _len(other._len), _input(other._input) {}

::parse::Error::Error(::parse::Error &&other)
    : _len(other._len), _input(other._input) {}

auto ::parse::Error::operator=(::parse::Error && other) -> Error & {
  _input = other._input;
  _len = other._len;
  return *this;
}

auto ::parse::Error::operator<(::parse::Error const &other) const -> bool {
  return _input.data() < other._input.data() ||
         (_input.data() == other._input.data() && _len < other._len);
}

auto ::parse::Error::len() const -> size_t { return _len; }

auto ::parse::Error::input() const -> ::parse::Input const & { return _input; }

auto ::parse::operator<<(std::ostream & out, ::parse::Error const &error)
    -> std::ostream & {
  static constexpr size_t context_size = 20;

  size_t offset = (error.input().data() - error.input().orig());
  if (offset > context_size) {
    offset = context_size;
  }

  size_t end = 0;
  while (*(error.input().data() + end) != '\0' && end <= context_size) {
    ++end;
  }

  out << "\033[1;31mERROR:\033[0m  "
      << std::string_view(error.input().data() - offset, offset + end)
      << (end > context_size ? "..." : "") << std::endl;
  out << "        " << std::string(offset, ' ') << "\033[1;31m"
      << std::string(std::min(end, error.len()), '^')
      << " Unexpected token.\033[0m" << std::endl;

  return out;
}
