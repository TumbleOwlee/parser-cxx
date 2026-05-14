#include "parser/indent.h"

#include <map>

static char const *const COLORS[] = {"\033[0;33m", "\033[0;34m", "\033[0;35m", "\033[0;36m"};

::parse::IndentLogger::IndentLogger(::parse::Input const &input, std::string_view func_name)
    : _input(input), _func_name(func_name) {
    auto &data = indents(std::this_thread::get_id());
    for (auto i = 0; i < data.level; ++i) {
        std::cerr << COLORS[i % 4] << "|   " << "\033[0m";
    }
    std::cerr << COLORS[data.level % 4] << "Entering " << _func_name << " with input: \033[1;32m" << _input.data()
              << "\033[0m" << std::endl;
    ++data.level;
}

::parse::IndentLogger::~IndentLogger() {
    auto &data = indents(std::this_thread::get_id());
    --data.level;
    for (auto i = 0; i < data.level; ++i) {
        std::cerr << COLORS[i % 4] << "|   " << "\033[0m";
    }
    std::cerr << COLORS[data.level % 4] << "Exiting " << _func_name << " with input: \033[1;31m" << _input.data()
              << "\033[0m" << std::endl;
}

auto ::parse::IndentLogger::indents(std::thread::id id) -> Data & {
    static std::map<std::thread::id, Data> _indents;
    return _indents[id];
}
