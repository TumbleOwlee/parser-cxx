#include "parser/value.h"

::parse::Value::Value() : std::variant<std::monostate, int, double, std::string, bool>() {}

auto ::parse::operator<<(std::ostream & out, ::parse::Value const &value) -> std::ostream & {
    if (auto v = std::get_if<std::string>(&value)) {
        out << "String { " << *v << " }";
    } else if (auto v = std::get_if<double>(&value)) {
        out << "Double { " << *v << " }";
    } else if (auto v = std::get_if<int>(&value)) {
        out << "Integer { " << *v << " }";
    } else if (auto v = std::get_if<bool>(&value)) {
        out << "Bool { " << *v << " }";
    }
    return out;
}

::parse::Value::operator bool() {
    if (auto v = std::get_if<std::string>(this)) {
        return !v->empty();
    } else if (auto v = std::get_if<double>(this)) {
        return *v;
    } else if (auto v = std::get_if<int>(this)) {
        return *v;
    } else if (auto v = std::get_if<bool>(this)) {
        return *v;
    }
    return false;
}
