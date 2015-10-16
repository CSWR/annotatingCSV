/**
 * @file forward_matcher.hpp
 * Modified implementation of the Aho-Corasick algorithm for string matching.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#ifndef FORWARD_MATCHER_H
#define FORWARD_MATCHER_H

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
struct fstate {
  std::array<fstate *, 256> transition; /**< Next state for each character. */
  fstate *fail;                         /**< Next state in case of failure. */
  size_t word_length;                  /**< Length of the word accepted by this
                                            state. Zero if doesn't accept. */
  fstate *dict_suffix;                  /**< Accept node that can be reached. */

  /**
   * Default state constructor.
   */
  fstate();
};

/**
 * Class for matching words on a string.
 */
class forward_matcher {
public:
  /**
   * Construct a matcher.
   * @param[in]  words Set of words to match.
   */
  explicit forward_matcher(const std::vector<std::string>& words);
  /**
   * Advance until a match occurs.
   * @param[in]  begin        String iterator where matching begins.
   * @param[in]  end          String iterator of document end.
   * @param[out] match_length Length of the match.
   */
  std::string::const_iterator advance_until_match(
      std::string::const_iterator begin, std::string::const_iterator end,
      size_t& match_length);

private:
  std::list<fstate> states_; /**< Holds all the states. */
  fstate *initial_; /**< Initial state. */
  fstate *current_; /**< Current state in the execution. */
  bool match_begin_;
  bool match_end_;

  /**
   * Advance the matcher with a character and store matches.
   * @param[in]  c       Character being read.
   * @returns size of the word matched.
   */
  size_t advance(unsigned char c);
  /**
   * Build the matcher. Must be called before scanning the document.
   */
  void build();
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
  void add_word(const std::string& word);
};

} // namespace eval
} // namespace navigational
} // namespace csvp

#endif
