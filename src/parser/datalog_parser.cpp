/**
 * @file datalog_parser.cpp
 * Parser for the datalog language.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */

#include "datalog_parser.hpp"
#include "datalog_grammar.hpp"

#include <algorithm>

namespace csvp {
namespace navigational {
namespace parser {

program datalog_parser::parse(const std::string& input) const {
  using namespace boost::spirit::qi;

  datalog_grammar<std::string::const_iterator> grammar;
  program program;
  auto iter = input.cbegin();
  auto end = input.cend();

  bool r = phrase_parse(iter, end, grammar, ascii::blank, program);

  if (r && iter == end)
    return program;
  else
    throw std::runtime_error("Errors ocurred during parsing.");
}

program datalog_parser::parse(std::istream& input) const {
  using namespace std;

  // Copy into string
  string storage;
  input.unsetf(ios::skipws);
  copy(istream_iterator<char>(input), istream_iterator<char>(),
       back_inserter(storage));

  // Call on string
  return parse(storage);
}

} // namespace parser
} // namespace navigational
} // namespace csvp
