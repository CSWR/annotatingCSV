/**
 * @file step1.hpp
 * Fills an array with the next matches for each position.
 * Correspond to "step1" in the eval algorithm.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#ifndef STEP1_H
#define STEP1_H

#include "reverse_matcher.hpp"
#include "pattern_normal_form.hpp"
#include <memory>

namespace csvp {
namespace navigational {
namespace eval {

/**
 * Holds matches and an array of next matches for each position.
 */
struct next_matches {
  std::vector<span> matches;
  std::vector<span*> next_match;
  bool filled;

  next_matches();
  span *& get(size_t index);
  void fill(size_t size);
};

using next_matches_ptr = std::shared_ptr<next_matches>;

struct var_matches {
  std::string variable;
  next_matches_ptr next_matches;

  var_matches(const std::string& variable_, next_matches_ptr next_matches_);
};

/**
 * Holds next matches for a whole group.
 */
struct group_matches {
  next_matches_ptr any;
  std::vector<var_matches> nexts;
  void fill(size_t size);
  size_t number_of_variables() const;
};

std::vector<group_matches> step1(const std::vector<pattern_group>& normal,
    std::string::const_iterator begin, std::string::const_iterator end);

} // namespace eval
} // namespace navigational
} // namespace csvp

#endif
