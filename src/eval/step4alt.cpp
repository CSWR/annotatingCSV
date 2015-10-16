/**
 * @file step4alt.cpp
 * Alternate step4 implementation with support for backrefences.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "step4alt.hpp"
#include "hash_table.hpp"
#include <cmath>
#include <cassert>

using namespace std;

namespace csvp {
namespace navigational {
namespace eval {

assignment_set step4alt(vector<group_c_match>& C, vector<size_t> R,
    size_t offset, const string& document) {
  assignment_set O(C.size());
  assignment_node *current = O.root();
  vector<size_t> T(C.size() + 2);

  hash_table table(document, offset, (size_t) sqrt(document.size()) + 1);

  assert(C.size() == 2);
  auto& x_c_match = C[0];
  auto& y_c_match = C[1];
  // Add y to hash
  for (auto& c_match : y_c_match.matches) {
    table.add(span(c_match.start, c_match.end), c_match.assignment);
  }

  for (auto& c_match : x_c_match.matches) {
    if (R[c_match.end] > 1) break;

    bool has_match = false;
    auto& variable = c_match.assignment[0].second;
    auto candidates = table.get(span(c_match.start, c_match.end));

    for (auto it = candidates.first; it != candidates.second; it++) {
      auto& other = it->assignment[0].second;
      if (!span_content_eq(variable, other, document, offset)) continue;

      // Add match
      if (!has_match) {
        has_match = true;
        current = assignment_set::add_child(current, c_match.assignment, offset);
      }
      current = assignment_set::add_child(current, it->assignment, offset);
      current = assignment_set::move_up(current);
    }

    if (has_match) {
      current = assignment_set::move_up(current);
    }
  }
  current = assignment_set::move_up(current);

  return O;
}

} // namespace eval
} // namespace navigational
} // namespace csvp
