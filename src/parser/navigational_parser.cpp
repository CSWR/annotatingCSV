/**
 * @file navigational_parser.cpp
 * Parser for the navigational language.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "navigational_parser.hpp"
#include "navigational_grammar.hpp"

#include <algorithm>

namespace csvp {
namespace navigational {
namespace parser {

pattern navigational_parser::parse(const std::string& input) const {
  using namespace boost::spirit::qi;

  navigational_grammar<std::string::const_iterator> grammar;
  pattern pattern;
  auto iter = input.cbegin();
  auto end = input.cend();

  bool r = phrase_parse(iter, end, grammar, ascii::blank, pattern);

  if (r && iter == end)
    return pattern;
  else
    throw std::runtime_error("Errors ocurred during parsing.");
}

pattern navigational_parser::parse(std::istream& input) const {
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
