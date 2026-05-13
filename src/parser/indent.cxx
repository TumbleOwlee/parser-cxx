#include "parser/indent.h"

#include <map>

::parse::IndentLogger::IndentLogger(::parse::Input const &input, std::string_view func_name)
    : _input(input), _func_name(func_name) {
    auto &data = indents(std::this_thread::get_id());
    ++data.level;
    std::cerr << "\033[0;32m" << std::string(data.level * 2, ' ') << "Entering " << _func_name
              << " with input: \033[1;32m" << _input.data() << "\033[0m" << std::endl;
}

::parse::IndentLogger::~IndentLogger() {
    auto &data = indents(std::this_thread::get_id());
    std::cerr << "\033[0;31m" << std::string(data.level * 2, ' ') << "Exiting " << _func_name
              << " with input: \033[1;31m" << _input.data() << "\033[0m" << std::endl;
    --data.level;
}

auto ::parse::IndentLogger::indents(std::thread::id id) -> Data & {
    static std::map<std::thread::id, Data> _indents;
    return _indents[id];
}
