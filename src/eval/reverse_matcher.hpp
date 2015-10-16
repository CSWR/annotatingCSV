/**
 * @file reverse_matcher.hpp
 * Modified implementation of the Aho-Corasick algorithm for string matching.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */

#ifndef REVERSE_MATCHER_H
#define REVERSE_MATCHER_H

#include <array>
#include <vector>
#include <list>
#include <string>
#include <utility>
#include "span.hpp"

namespace csvp {
namespace navigational {
namespace eval {

using match_vector = std::vector<span>;

/**
 * State of the trie.
 */
struct state {
  std::array<state *, 256> transition; /**< Next state for each character. */
  state *fail;                         /**< Next state in case of failure. */
  size_t word_length;                  /**< Length of the word accepted by this
                                            state. Zero if doesn't accept. */
  state *dict_suffix;                  /**< Accept node that can be reached. */
  std::vector<match_vector *> dests;   /**< Destinations for matches. */

  /**
   * Default state constructor.
   */
  state();
  /**
   * Add match to all destinations.
   */
  void add_match(size_t pos) const;
};

/**
 * Class for matching words on a string.
 */
class reverse_matcher {
public:
  /**
   * Construct a matcher.
   */
  reverse_matcher();

  /**
   * Add a set of words associated with a destination for the matches.
   * @param[in]  words Set of words to match.
   * @param[out] dest  Vector where matches for this will be stored.
   */
  void add_set_of_words(const std::vector<std::string>& words,
      match_vector& dest);
  /**
   * Build the matcher. Must be called before scanning the document.
   */
  void build();
  /**
   * Advance the matcher with a character and store matches.
   * @param[in]  pos     Position in the text.
   * @param[in]  c       Character being read.
   * @returns wether there was a match.
   */
  bool advance(size_t pos, unsigned char c);
  /**
   * Scan document for matches.
   * @param[in] begin Begin of document that will be scanned.
   * @param[in] end   End of document that will be scanned.
   */
  void scan(std::string::const_iterator begin, std::string::const_iterator end);
  bool advance_begin(size_t pos);
  bool advance_end(size_t pos);

private:
  std::list<state> states_; /**< Holds all the states. */
  state *initial_; /**< Initial state. */
  state *current_; /**< Current state in the execution. */
  bool built_; /**< Wether we have built the matcher. */
  std::vector<match_vector *> match_begin_;
  std::vector<match_vector *> match_end_;

  /**
   * Builds the initial state.
   */
  void build_initial_state();
  /**
   * Build the transition function for the initial state.
   */
  void build_initial_transitions();
  /**
   * Builds the fail function.
   */
  void build_fail_function();
  /**
   * Optimizes the fail function.
   */
  void optimize_fail_function();
  /**
   * Builds the dictionary suffix function.
   */
  void build_dict_suffix();
  /**
   * Adds a word to the trie.
   */
  void add_word(const std::string& word, match_vector& dest);
};

}
}
}

#endif
