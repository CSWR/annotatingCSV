/**
 * @file step4.cpp
 * Step four of the core algorithm.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "step4.hpp"

using namespace std;

namespace csvp {
namespace navigational {
namespace eval {

assignment_set step4(vector<group_c_match>& C, vector<size_t> R, size_t offset) {
  assignment_set O(C.size());
  assignment_node *current = O.root();
  vector<size_t> T(C.size() + 2);
  T[1] = 0;
  size_t m = 0;

  while (m != static_cast<size_t>(-1)) {
    if (R[T[m + 1]] > m) {
      current = assignment_set::move_up(current);
      m--;
      T[m + 1]++;
    }
    else {
      c_match& c = *C[m][T[m + 1]];
      T[m + 1] = c.start;
      current = assignment_set::add_child(current, c.assignment, offset);
      m++;
      T[m + 1] = c.end;
    }
    if (m == C.size()) {
      current = assignment_set::move_up(current);
      m--;
      T[m + 1]++;
    }
  }

  return O;
}

} // namespace eval
} // namespace navigational
} // namespace csvp
