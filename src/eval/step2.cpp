/**
 * @file step2.cpp
 * Step two of the core algorithm.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */

#include "step2.hpp"

#include <cassert>

using namespace std;

namespace csvp {
namespace navigational {
namespace eval {

c_match::c_match(size_t start_, size_t end_,
    vector<pair<string, span>>&& assignment_)
  : start(start_), end(end_), assignment(move(assignment_)) {}

bool c_match::operator==(const c_match& rhs) const {
  return start == rhs.start
      && end == rhs.end
      && assignment == rhs.assignment;
}

group_c_match::group_c_match(size_t size) : next_match(size + 1, nullptr) {}

c_match *& group_c_match::operator[](size_t index) {
  return next_match[index];
}

void group_c_match::fill(size_t from, size_t to, size_t start, size_t end,
    vector<pair<string, span>>&& assignment) {
  assert(from <= to);

  matches.emplace_back(start, end, move(assignment));
  c_match *c = &matches.back();

  for (size_t i = from; i <= to; i++) {
    next_match[i] = c;
  }
}

group_c_match step2_single(group_matches& matches,
    string::const_iterator begin, string::const_iterator end) {
  size_t m = 0;
  auto& any_matches = matches.any;
  auto& nexts_matches = matches.nexts;

  // Fill arrays
  size_t document_size = distance(begin, end);
  any_matches->fill(document_size);
  for (auto& next : nexts_matches) next.next_matches->fill(document_size);

  group_c_match C(any_matches->next_match.size());
  size_t number_of_vars = matches.number_of_variables();

  while (m < any_matches->next_match.size()
      && any_matches->get(m)) {
    auto s = any_matches->get(m);
    size_t r1 = s->start;
    vector<pair<string, span>> sigma;
    sigma.reserve(number_of_vars);

    bool flag = true;
    for (auto& next : nexts_matches) {
      auto& t = next.next_matches->get(s->end);
      if (!t) {
        flag = false;
        break;
      }
      if (!next.variable.empty()) {
        sigma.emplace_back(next.variable, span(s->end, t->start));
      }
      s = t;
    }

    if (flag) {
      size_t r2 = s->end;
      C.fill(m, r1, r1, r2, move(sigma));
      m = r1 + 1;
    }
    else if (r1 + 1 < any_matches->next_match.size()
        && any_matches->get(r1 + 1)
        && any_matches->get(r1 + 1)->end <= any_matches->get(r1)->end) {
      any_matches->get(m) = any_matches->get(r1 + 1);
    }
    else {
      break;
    }
  }

  // Free memory
  matches.any.reset();
  for (auto& next : matches.nexts)
    next.next_matches.reset();

  return C;
}

vector<group_c_match> step2(vector<group_matches>& matches,
    string::const_iterator begin, string::const_iterator end) {
  vector<group_c_match> result;
  result.reserve(matches.size());

  for (auto& match : matches) {
    result.emplace_back(step2_single(match, begin, end));
  }

  return result;
}

} // namespace eval
} // namespace navigational
} // namespace csvp
