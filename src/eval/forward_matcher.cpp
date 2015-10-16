/**
 * @file forward_matcher.cpp
 * Modified implementation of the Aho-Corasick algorithm for string matching.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "forward_matcher.hpp"
#include <queue>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

namespace csvp {
namespace navigational {
namespace eval {

fstate::fstate() : transition({}), fail(nullptr), word_length(0),
  dict_suffix(nullptr) {}

forward_matcher::forward_matcher(const vector<string>& words)
: match_begin_(false), match_end_(false) {
  build_initial_state();
  for (auto& word : words) add_word(word);
  build();
}

void forward_matcher::build() {
  build_initial_transitions();
  build_fail_function();
  build_dict_suffix();
  optimize_fail_function();
}

size_t forward_matcher::advance(unsigned char c) {
  // Advance
  while (!current_->transition[c])
    current_ = current_->fail;
  current_ = current_->transition[c];

  // Accept state
  if (current_->word_length) {
    return current_->word_length;
  }

  // Follow suffixes
  fstate *dict_suffix_state = current_->dict_suffix;
  while (dict_suffix_state) {
    // Should be an accepting state
    assert(dict_suffix_state->word_length);

    // Add match and continue
    return dict_suffix_state->word_length;
  }

  return 0;
}

string::const_iterator forward_matcher::advance_until_match(
    string::const_iterator begin, string::const_iterator end,
    size_t& match_length) {
  if (begin == end) {
    match_length = 0;
    return end;
  }

  do {
    match_length = advance(*begin);
    begin++;
  } while (!match_length && begin != end);

  if (begin == end && match_end_) {
    match_length = 0;
    return end;
  }

  return begin;
}

void forward_matcher::build_initial_state() {
  // Add initial state
  states_.emplace_back();
  initial_ = &states_.back();
  current_ = initial_;
}

void forward_matcher::build_initial_transitions() {
  // Add self transitions
  for (auto& s : initial_->transition) if (!s) s = initial_;
}

void forward_matcher::build_fail_function() {
  // BFS queue
  queue<fstate *> q;

  // Depth one
  initial_->fail = initial_;
  for (fstate *s : initial_->transition) {
    if (s == initial_) continue;
    s->fail = initial_;
    q.push(s);
  }

  while (!q.empty()) {
    fstate *s = q.front();
    q.pop();

    for (unsigned a = 0; a < 256; a++) {
      fstate *t = s->transition[a];
      if (!t) continue;

      fstate *r = s->fail;
      while (!r->transition[a]) r = r->fail;

      t->fail = r->transition[a];
      q.push(t);
    }
  }
}

void forward_matcher::optimize_fail_function() {
  queue<fstate *> q;

  for (fstate *s : initial_->transition)
    if (s != initial_) q.push(s);

  while (!q.empty()) {
    fstate *s = q.front();
    q.pop();
    fstate *fail_state = s->fail;

    bool is_subset = true;
    for (unsigned a = 0; a < 256; a++) {
      if (fail_state->transition[a] && !s->transition[a]) {
        is_subset = false;
        break;
      }
    }

    if (is_subset)
      s->fail = fail_state->fail;
  }
}

void forward_matcher::build_dict_suffix() {
  queue<fstate *> q;
  for (fstate *s : initial_->transition)
    if (s != initial_) q.push(s);

  while (!q.empty()) {
    fstate *s = q.front();
    q.pop();

    if (s->fail->word_length) {
      // s is an accepting state
      s->dict_suffix = s->fail;
    } else if (s->fail->dict_suffix) {
      s->dict_suffix = s->fail->dict_suffix;
    }

    for (auto t : s->transition)
      if (t) q.push(t);
  }
}

void forward_matcher::add_word(const string& word) {
  fstate *current = initial_;
  string mword = word;

  // Check for anchors
  if (mword == "^") {
    match_begin_ = true;
    return;
  } else if (mword == "$") {
    match_end_ = true;
    return;
  } else if (mword == "\\^") {
    mword = "^";
  } else if (mword == "\\$") {
    mword = "$";
  }

  // Transverse word
  for (auto it = mword.begin(); it != mword.end(); it++) {
    unsigned char index = *it & 0xFF;
    fstate *next = current->transition[index];

    if (next) {
      // Go to next state
      current = next;
    } else {
      // Create next state
      states_.emplace_back();
      current->transition[index] = &states_.back();
      current = &states_.back();
    }
  }

  // Mark as finish state
  current->word_length = mword.size();
}
} // namespace eval
} // namespace navigational
} // namespace csvp
