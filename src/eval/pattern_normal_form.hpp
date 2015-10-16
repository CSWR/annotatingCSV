/**
 * @file pattern_normal_form.hpp
 * Converts a pattern into normal form.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#ifndef PATTERN_NORMAL_FORM_H
#define PATTERN_NORMAL_FORM_H

#include "../parser/navigational_parser_types.hpp"

namespace csvp {
namespace navigational {
namespace eval {

/**
 * Represents a group of the form
 * (any(S) /v:next(S)* /x:next(S) /v:next(S)*) where v empty or a variable.
 */
struct pattern_group {
  parser::pattern_any any;
  std::vector<parser::pattern_variable> nexts;

  pattern_group() {}
  pattern_group(parser::pattern_any any_,
      std::vector<parser::pattern_variable> nexts_)
    : any(any_), nexts(nexts_) {}

  bool operator==(const pattern_group& rhs) const;
};

/**
 * Represent a pattern in normal form.
 * (v:next(S))* (<pattern_group>)*
 */
struct normal_pattern {
  std::vector<parser::pattern_variable> prefix;
  std::vector<pattern_group> groups;

  normal_pattern() {}
  normal_pattern(std::vector<parser::pattern_variable> prefix_,
      std::vector<pattern_group> groups_)
    : prefix(prefix_), groups(groups_) {}

  bool operator==(const normal_pattern& rhs) const;
};

/**
 * Transforms a pattern into normal form.
 * @param[in] pattern will be transformed.
 * @returns transformed pattern.
 */
normal_pattern pattern_normal_form(parser::pattern& pattern);

} // namespace eval
} // namespace navigational
} // namespace csvp

#endif
