/**
 * @file reverse_matcher.cpp
 * Modified implementation of the Aho-Corasick algorithm for string matching.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */

#include "reverse_matcher.hpp"

#include <algorithm>
#include <queue>
#include <cassert>

using namespace std;

namespace csvp {
namespace navigational {
namespace eval {

state::state() : transition({}), fail(nullptr), word_length(0),
  dict_suffix(nullptr) {}

void state::add_match(size_t pos) const {
  for (auto dest : dests) {
    dest->emplace_back(pos, pos + word_length);
  }
}

reverse_matcher::reverse_matcher() : built_(false) {
  build_initial_state();
}

void reverse_matcher::add_set_of_words(const vector<string>& words,
    match_vector& dest) {
  for (auto& word : words) add_word(word, dest);
}

void reverse_matcher::build() {
  if (built_) return;

  build_initial_transitions();
  build_fail_function();
  build_dict_suffix();
  optimize_fail_function();

  built_ = true;
}

bool reverse_matcher::advance(size_t pos, unsigned char c) {
  // Must be built before executing.
  assert(built_);

  // Advance
  while (!current_->transition[c])
    current_ = current_->fail;
  current_ = current_->transition[c];

  // Accept state
  if (current_->word_length) {
    current_->add_match(pos);
  }

  // Follow suffixes
  state *dict_suffix_state = current_->dict_suffix;
  while (dict_suffix_state) {
    // Should be an accepting state
    assert(dict_suffix_state->word_length);

    // Add match and continue
    dict_suffix_state->add_match(pos);
    dict_suffix_state = dict_suffix_state->dict_suffix;
  }

  return current_->word_length || current_->dict_suffix;
}

void reverse_matcher::scan(string::const_iterator begin,
    string::const_iterator end) {
  size_t pos = distance(begin, end);
  /* for (auto it = document.crbegin(); it != document.crend(); it++) { */
  for (auto it = end - 1; it != begin - 1; it--) {
    pos--;
    advance(pos, *it);
  }
}

bool reverse_matcher::advance_begin(size_t pos) {
  for (auto dest : match_begin_) {
    dest->emplace_back(pos, pos);
  }
  return match_begin_.size();
}

bool reverse_matcher::advance_end(size_t pos) {
  for (auto dest : match_end_) {
    dest->emplace_back(pos, pos);
  }
  return match_end_.size();
}

void reverse_matcher::build_initial_state() {
  // Add initial state
  states_.emplace_back();
  initial_ = &states_.back();
  current_ = initial_;
}

void reverse_matcher::build_initial_transitions() {
  // Add self transitions
  for (auto& s : initial_->transition) if (!s) s = initial_;
}

void reverse_matcher::build_fail_function() {
  // BFS queue
  queue<state *> q;

  // Depth one
  initial_->fail = initial_;
  for (state *s : initial_->transition) {
    if (s == initial_) continue;
    s->fail = initial_;
    q.push(s);
  }

  while (!q.empty()) {
    state *s = q.front();
    q.pop();

    for (unsigned a = 0; a < 256; a++) {
      state *t = s->transition[a];
      if (!t) continue;

      state *r = s->fail;
      while (!r->transition[a]) r = r->fail;

      t->fail = r->transition[a];
      q.push(t);
    }
  }
}

void reverse_matcher::optimize_fail_function() {
  queue<state *> q;

  for (state *s : initial_->transition)
    if (s != initial_) q.push(s);

  while (!q.empty()) {
    state *s = q.front();
    q.pop();
    state *fail_state = s->fail;

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

void reverse_matcher::build_dict_suffix() {
  queue<state *> q;
  for (state *s : initial_->transition)
    if (s != initial_) q.push(s);

  while (!q.empty()) {
    state *s = q.front();
    q.pop();

    if (s->fail->word_length) {
      // s is an accepting state
      s->dict_suffix = s->fail;
    }
    else if (s->fail->dict_suffix) {
      s->dict_suffix = s->fail->dict_suffix;
    }

    for (auto t : s->transition)
      if (t) q.push(t);
  }
}

void reverse_matcher::add_word(const string& word, match_vector& dest) {
  state *current = initial_;
  string mword = word;

  if (mword == "^") {
    match_begin_.push_back(&dest);
    return;
  } else if (mword == "$") {
    match_end_.push_back(&dest);
    return;
  } else if (mword == "\\^") {
    mword = "^";
  } else if (mword == "\\$") {
    mword = "$";
  }

  // Transverse word in reverse
  for (auto it = mword.rbegin(); it != mword.rend(); it++) {
    unsigned char index = *it & 0xFF;
    state *next = current->transition[index];

    if (next) {
      // Go to next state
      current = next;
    }
    else {
      // Create next state
      states_.emplace_back();
      current->transition[index] = &states_.back();
      current = &states_.back();
    }
  }

  // Mark as finish state
  current->word_length = mword.size();
  current->dests.push_back(&dest);
}

} // namespace eval
} // namespace navigational
} // namespace csvp
