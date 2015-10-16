/**
 * @file navigational_formula_eval.cpp
 * Evaluates navigational formulas.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "navigational_formula_eval.hpp"

#include "../logger.hpp"
#include "prefix_step.hpp"
#include "step1.hpp"
#include "step2.hpp"
#include "step3.hpp"
#include "step4.hpp"
#include "step4alt.hpp"

using namespace std;

namespace csvp {
namespace navigational {
namespace eval {

assignment_set navigational_formula_eval(parser::pattern& pattern,
    const string& document, vector<pair<string, span>>& prefix_assignment,
    bool benchmark) {
  // Check for backreferences
  bool alternate_algorithm = pattern.has_backreference();

  // Transform to normal form
  normal_pattern normal = pattern_normal_form(pattern);

  // Match prefix
  auto prefix_end = prefix_step(normal.prefix, prefix_assignment,
      document.begin(), document.end());
  size_t offset = distance(document.begin(), prefix_end);

  // Quit if there are no groups
  if (normal.groups.empty())
    return assignment_set(0);

  logger l(benchmark);

  auto B = step1(normal.groups, prefix_end, document.end());
  l.log("Step 1");

  auto C = step2(B, prefix_end, document.end());
  l.log("Step 2");

  auto R = step3(C, prefix_end, document.end());
  l.log("Step 3");

  auto ans = alternate_algorithm
    ? step4alt(C, R, offset, document)
    : step4(C, R, offset);
  l.log("Step 4");

  return ans;
}

} // namespace eval
} // namespace navigational
} // namespace csvp
