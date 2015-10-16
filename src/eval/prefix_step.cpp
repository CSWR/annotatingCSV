/**
 * @file prefix_step.cpp
 * Matches the pattern prefix to a document.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "prefix_step.hpp"
#include "forward_matcher.hpp"

using namespace std;
using namespace csvp::navigational::parser;

namespace csvp {
namespace navigational {
namespace eval {

string::const_iterator prefix_step(
    vector<pattern_variable>& prefix,
    vector<pair<string, span>>& result,
    string::const_iterator begin,
    string::const_iterator end) {
  auto it = begin;
  for (auto& next : prefix) {
    if (it == end) break;

    size_t match_length;
    forward_matcher matcher(next.next.alternation.words);
    auto new_it = matcher.advance_until_match(it, end, match_length);
    if (next.variable.size()) {
      result.emplace_back(next.variable,
          span(distance(begin, it), distance(begin, new_it - match_length)));
    }
    it = new_it;
  }
  return it;
}

} // namespace eval
} // namespace navigational
} // namespace csvp
