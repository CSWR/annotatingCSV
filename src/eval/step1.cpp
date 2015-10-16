/**
 * @file step1.cpp
 * Fills an array with the next matches for each position.
 * Correspond to "step1" in the eval algorithm.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "step1.hpp"
#include <map>
#include <cassert>

using namespace std;

/**
 * For comparing sets of words.
 */
namespace std {
template<>
struct less<const vector<string>*> {
  bool operator()(const vector<string> *lhs, const vector<string> *rhs) const {
    return lexicographical_compare(lhs->cbegin(), lhs->cend(),
        rhs->cbegin(), rhs->cend());
  }
};
}

namespace csvp {
namespace navigational {
namespace eval {

next_matches::next_matches()
  : filled(false) {}

span *& next_matches::get(size_t index) {
  assert(filled);
  return next_match[index];
}

using word_set_ptr = const vector<string>*;

/**
 * Creates a new match vector for a word set or shares it if it already exists.
 */
next_matches_ptr add_word_set(map<word_set_ptr, next_matches_ptr>& assoc,
    reverse_matcher& matcher, word_set_ptr words) {
  auto it = assoc.find(words);

  // Already exists
  if (it != assoc.end()) {
    return it->second;
  }

  // Create new
  auto next_m = make_shared<next_matches>();
  assoc.insert(make_pair(words, next_m));

  // Add words to matcher
  matcher.add_set_of_words(*words, next_m->matches);

  return next_m;
}

/**
 * Build containers for matches and add words to matcher.
 */
void build_containers(vector<group_matches>& result, reverse_matcher& matcher,
    const vector<pattern_group>& normal) {
  map<word_set_ptr, next_matches_ptr> assoc;

  // Groups
  result.reserve(normal.size());
  for (auto& group : normal) {
    result.emplace_back();
    auto& group_matches = result.back();

    // Any
    group_matches.any = add_word_set(assoc, matcher,
        &group.any.alternation.words);

    // Nexts
    group_matches.nexts.reserve(group.nexts.size());
    for (auto& next : group.nexts) {
      auto dest = add_word_set(assoc, matcher, &next.next.alternation.words);
      group_matches.nexts.emplace_back(next.variable, dest);
    }
  }
}

/**
 * Run matcher to scan document.
 */
void run_matcher(reverse_matcher& matcher, string::const_iterator begin,
    string::const_iterator end, bool from_the_start) {
  // Finish build
  matcher.build();

  // Scan document
  matcher.advance_end(distance(begin, end));
  matcher.scan(begin, end);
  if (from_the_start) matcher.advance_begin(0);
}

/**
 * Fill an array of next_matches.
 */
void next_matches::fill(size_t size) {
  if (filled) return;

  span *last_match = nullptr;
  next_match.resize(size + 1);
  size_t next_idx = size;

  // For each match
  for (auto& match : matches) {
    // Fill until start
    while (next_idx != match.start) {
      next_match[next_idx] = last_match;
      next_idx--;
    }

    last_match = &match;
  }
  // Last portion
  while (next_idx > 0) {
    next_match[next_idx] = last_match;
    next_idx--;
  }
  // Position 0
  next_match[next_idx] = last_match;

  filled = true;
}

var_matches::var_matches(const string& variable_, next_matches_ptr next_matches_)
  : variable(variable_), next_matches(next_matches_) {}

/**
 * Fill all next_matches in a group.
 */
void group_matches::fill(size_t size) {
  any->fill(size);

  for (auto& next : nexts)
    next.next_matches->fill(size);
}

/**
 * Returns the number of variables in the group.
 */
size_t group_matches::number_of_variables() const {
  size_t result = 0;
  for (auto& next : nexts)
    if (!next.variable.empty())
      result++;
  return result;
}

/**
 * Find all matches for the word sets in the pattern.
 */
vector<group_matches> step1(const vector<pattern_group>& normal,
    std::string::const_iterator begin, std::string::const_iterator end) {
  vector<group_matches> result;
  reverse_matcher matcher;

  build_containers(result, matcher, normal);
  // Assume that it's always from the start
  run_matcher(matcher, begin, end, true);

  return result;
}

} // namespace eval
} // namespace navigational
} // namespace csvp
