/**
 * @file step2.hpp
 * Step two of the core algorithm.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */

#ifndef STEP2_H
#define STEP2_H

#include "step1.hpp"

#include <list>

namespace csvp {
namespace navigational {
namespace eval {

struct c_match {
  size_t start;
  size_t end;
  std::vector<std::pair<std::string, span>> assignment;

  c_match(size_t start_, size_t end_,
      std::vector<std::pair<std::string, span>>&& assignment_);
  bool operator==(const c_match& rhs) const;
};

struct group_c_match {
  std::list<c_match> matches;
  std::vector<c_match*> next_match;

  group_c_match(size_t size);
  c_match *& operator[](size_t index);
  void fill(size_t from, size_t to, size_t start, size_t end,
      std::vector<std::pair<std::string, span>>&& assignment);
};

group_c_match step2_single(group_matches& matches,
    std::string::const_iterator begin, std::string::const_iterator end);
std::vector<group_c_match> step2(std::vector<group_matches>& matches,
    std::string::const_iterator begin, std::string::const_iterator end);

} // namespace eval
} // namespace navigational
} // namespace csvp

#endif
