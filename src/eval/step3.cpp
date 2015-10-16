/**
 * @file step3.cpp
 * Step three in the core algorithm.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */

#include "step3.hpp"

using namespace std;

namespace csvp {
namespace navigational {
namespace eval {

vector<size_t> step3(std::vector<group_c_match>& pattern_c_match,
    string::const_iterator begin, string::const_iterator end) {
  size_t m = pattern_c_match.size() - 1;
  size_t length = distance(begin, end);
  vector<size_t> R(length + 1);

  // Iterate from length to 0
  for (size_t i = length + 1; i-- > 0;) {
    if (m == static_cast<size_t>(-1)) {
      R[i] = 0;
    }
    else if (!pattern_c_match[m][i]) {
      R[i] = m + 1;
    }
    else {
      c_match& c = *pattern_c_match[m][i];
      if (R[c.end] == m + 1) m--;
      R[i] = m + 1;
    }
  }
  return R;
}

} // namespace eval
} // namespace navigational
} // namespace csvp
