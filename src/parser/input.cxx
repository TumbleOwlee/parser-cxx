#include "parser/input.h"

::parse::Input::Input(char const *ptr) : _orig(ptr), _ptr(ptr) {}

auto ::parse::Input::data() const -> char const * { return _ptr; }

auto ::parse::Input::orig() const -> char const * { return _orig; }

auto ::parse::Input::operator++() -> ::parse::Input & {
    if (*_ptr != '\0') {
        ++_ptr;
    }
    return *this;
}

auto ::parse::Input::operator+=(size_t offset) -> ::parse::Input & {
    for (size_t i = 0; i < offset && *_ptr != '\0'; ++i) {
        ++_ptr;
    }
    return *this;
}

auto ::parse::Input::operator=(::parse::Input const &other) -> ::parse::Input & {
    _ptr = other._ptr;
    return *this;
}

auto ::parse::Input::operator+(size_t offset) const -> char const * {
    auto new_ptr = _ptr;
    for (size_t i = 0; i < offset && *new_ptr != '\0'; ++i) {
        ++new_ptr;
    }
    return new_ptr;
}

auto ::parse::Input::operator-(size_t offset) const -> char const * { return _ptr - offset; }

auto ::parse::Input::operator*() const -> char { return *_ptr; }

auto ::parse::Input::operator++(int) -> ::parse::Input & {
    ++_ptr;
    return *this;
}

auto ::parse::Input::operator==(::parse::Input const &other) const -> bool { return _ptr == other._ptr; }

auto ::parse::Input::operator!=(::parse::Input const &other) const -> bool { return _ptr != other._ptr; }
