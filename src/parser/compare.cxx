#include "parser/compare.h"

#include <string>
#include <variant>

template <typename T, typename S>
struct Context {
    static inline auto eval(T const &left, ::parse::compare::Comparison comparison, S const &right) -> bool {
        switch (comparison) {
        case ::parse::compare::Comparison::Less:
            return left < right;
        case ::parse::compare::Comparison::LessEqual:
            return left <= right;
        case ::parse::compare::Comparison::Equal:
            return left == right;
        case ::parse::compare::Comparison::NotEqual:
            return left != right;
        case ::parse::compare::Comparison::Greater:
            return left > right;
        case ::parse::compare::Comparison::GreaterThan:
            return left >= right;
        defaut:
            return false;
        }
        return false;
    }
};

auto ::parse::compare::eval(::parse::Value const &left, ::parse::compare::Comparison op, ::parse::Value const &right)
    -> bool {

#define apply_compare_type(T, S)                                                                                       \
    do {                                                                                                               \
        {                                                                                                              \
            T const *v1 = std::get_if<T>(&left);                                                                       \
            S const *v2 = std::get_if<S>(&right);                                                                      \
            if (v1 != nullptr && v2 != nullptr) {                                                                      \
                return Context<T, S>::eval(*v1, op, *v2);                                                              \
            }                                                                                                          \
        }                                                                                                              \
        {                                                                                                              \
            S const *v1 = std::get_if<S>(&left);                                                                       \
            T const *v2 = std::get_if<T>(&right);                                                                      \
            if (v1 != nullptr && v2 != nullptr) {                                                                      \
                return Context<S, T>::eval(*v1, op, *v2);                                                              \
            }                                                                                                          \
        }                                                                                                              \
    } while (false)

    // Types:
    //  - double
    //  - int
    //  - bool
    //  - std::string

    apply_compare_type(int, int);
    apply_compare_type(int, double);
    apply_compare_type(int, bool);

    apply_compare_type(double, double);
    apply_compare_type(double, bool);

    apply_compare_type(std::string, std::string);

#undef apply_compare_type

    return false;
}
