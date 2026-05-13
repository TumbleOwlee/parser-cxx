#include <iostream>

#include "parser.h"

int main(int argc, char **argv) {
  char const *const STATEMENT =
      "if (1.1 == 1) { if (3 == 3) { 11 } else { 22 } } else { 33 } ";
  std::cerr << "Parsing: '" << STATEMENT << "'" << std::endl;
  auto res = parse(STATEMENT);
  std::cerr << "Result: " << (res ? "SUCCESS" : "ERROR") << std::endl;
  return 0;
}
