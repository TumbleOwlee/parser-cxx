#include "parser/layer.h"

auto ::parse::operator<<(std::ostream & out, parse::Layer const &layer) -> std::ostream & {
    if (auto v = std::get_if<parse::Value>(&layer)) {
        out << "ValueType { " << *v << " }";
    } else if (auto v = std::get_if<::parse::compare::Comparison>(&layer)) {
        out << "Operator { ";
        switch (*v) {
        case ::parse::compare::Comparison::Less:
            out << "<";
            break;
        case ::parse::compare::Comparison::LessEqual:
            out << "<=";
            break;
        case ::parse::compare::Comparison::Equal:
            out << "==";
            break;
        case ::parse::compare::Comparison::NotEqual:
            out << "!=";
            break;
        case ::parse::compare::Comparison::Greater:
            out << ">";
            break;
        case ::parse::compare::Comparison::GreaterThan:
            out << ">=";
            break;
        }
        out << " }";
    } else {
        out << "Empty Layer";
    }
    return out;
}

auto ::parse::operator<<(std::ostream & out, std::expected<std::vector<parse::Layer>, parse::Error> const &result)
    -> std::ostream & {
    if (result) {
        std::cerr << "\033[1;32mSUCCESS:\033[0m " << std::endl;
        for (auto const &v : result.value()) {
            std::cerr << "    - " << v << std::endl;
        }
    } else {
        std::cerr << result.error();
    }
    return out;
}
