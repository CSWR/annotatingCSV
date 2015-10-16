/*
 * Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "catch.hpp"

#include "eval/reverse_matcher.hpp"

using namespace csvp::navigational::eval;
using namespace std;

TEST_CASE("the string matcher works correctly", "[reverse_matcher]") {
  SECTION("Test string matching") {
    vector<string> words({"a","ab","bab","bc","bca","c","caa"});
    string subject("abccab");
    reverse_matcher matcher;
    vector<span> matches;

    matcher.add_set_of_words(words, matches);
    matcher.build();

    matcher.scan(subject.begin(), subject.end());

    vector<span> solution;
    solution.reserve(7);
    solution.emplace_back(4, 4 + words[1].size());
    solution.emplace_back(4, 4 + words[0].size());
    solution.emplace_back(3, 3 + words[5].size());
    solution.emplace_back(2, 2 + words[5].size());
    solution.emplace_back(1, 1 + words[3].size());
    solution.emplace_back(0, words[1].size());
    solution.emplace_back(0, words[0].size());

    REQUIRE(matches == solution);
  }

  SECTION("Test matching order") {
    vector<string> words({"a","bab","bbabb","bbbabbb"});
    string subject("bbbbabbbb");
    reverse_matcher matcher;
    vector<span> matches;
    matcher.add_set_of_words(words, matches);
    matcher.build();

    matcher.scan(subject.begin(), subject.end());

    vector<span> solution;
    solution.emplace_back(4, 4 + words[0].size());
    solution.emplace_back(3, 3 + words[1].size());
    solution.emplace_back(2, 2 + words[2].size());
    solution.emplace_back(1, 1 + words[3].size());

    REQUIRE(matches == solution);
  }
}
