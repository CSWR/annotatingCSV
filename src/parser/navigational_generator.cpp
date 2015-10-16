/**
 * @file navigational_generator.cpp
 * Generator for printing the navigational language.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */

#include "navigational_generator.hpp"
#include "navigational_print_grammar.hpp"
#include <cassert>

namespace csvp {
namespace navigational {
namespace parser {

using namespace std;

string navigational_generator::generate(const pattern& pattern) const {
  using back_it = back_insert_iterator<string>;

  string ret;
  back_it sink(ret);
  navigational_print_grammar<back_it> grammar;

  auto success = karma::generate(sink, grammar, pattern);
  assert(success);

  return ret;
}

}
}
}
