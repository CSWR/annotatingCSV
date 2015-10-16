/*
 * Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "catch.hpp"

#include "eval/step1.hpp"

using namespace csvp::navigational::eval;
using namespace csvp::navigational::parser;
using namespace std;

TEST_CASE("step one works correctly", "[step1]") {
  SECTION("Test with example") {
    vector<string> words({"a","bab","bbabb","bbbabbb"});
    string subject("bbbbabbbb");

    vector<pattern_group> groups;
    groups.emplace_back(pattern_any(pattern_alternation(words)),
        vector<pattern_variable>());

    auto result = step1(groups, subject.begin(), subject.end());
    result[0].fill(subject.size());
    auto& next_match = result[0].any->next_match;

    REQUIRE(*next_match[0] == span(1, 1 + words[3].size()));
    REQUIRE(*next_match[1] == span(1, 1 + words[3].size()));
    REQUIRE(*next_match[2] == span(2, 2 + words[2].size()));
    REQUIRE(*next_match[3] == span(3, 3 + words[1].size()));
    REQUIRE(*next_match[4] == span(4, 4 + words[0].size()));
    REQUIRE(next_match[5] == nullptr);
    REQUIRE(next_match[6] == nullptr);
    REQUIRE(next_match[7] == nullptr);
    REQUIRE(next_match[8] == nullptr);
    REQUIRE(next_match[9] == nullptr);
  }

  SECTION("Test word set optimization") {
    vector<string> words1({"a","bab","bbabb","bbbabbb"});
    vector<string> words2({"a","bab","bbabb","bbbabbb"});
    string subject("bbbbabbbb");

    vector<pattern_group> groups;
    groups.emplace_back(pattern_any(pattern_alternation(words1)),
        vector<pattern_variable>(
          {pattern_variable("", pattern_next(pattern_alternation(words2)))}));

    auto result = step1(groups, subject.begin(), subject.end());
    result[0].fill(subject.size());
    auto& next_match1 = result[0].any;
    auto& next_match2 = result[0].nexts[0].next_matches;

    REQUIRE(next_match1 == next_match2);
  }
}
