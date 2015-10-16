/*
 * Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "catch.hpp"

#include "eval/pattern_normal_form.hpp"
#include "parser/navigational_parser.hpp"

using namespace csvp::navigational::eval;
using namespace csvp::navigational::parser;
using namespace std;

TEST_CASE("patterns transform correctly into normal form", "[pattern_normal_form]") {
  navigational_parser parser;

  SECTION("Test transforming a pattern with no variables") {
    string pattern_s("/any(a)/next(b)/any(c)/");
    pattern pat = parser.parse(pattern_s);
    normal_pattern given = pattern_normal_form(pat);

    normal_pattern expected;
    expected.prefix.emplace_back(
        pattern_variable("",
          pattern_next(
            pattern_alternation(
              vector<string>({"a"})))));
    expected.prefix.emplace_back(
        pattern_variable("",
          pattern_next(
            pattern_alternation(
              vector<string>({"b"})))));
    expected.prefix.emplace_back(
        pattern_variable("",
          pattern_next(
            pattern_alternation(
              vector<string>({"c"})))));

    REQUIRE(given == expected);
  }

  SECTION("Test transforming a pattern with one group") {
    string pattern_s("/any(a)/next(b)/any(c)/?x:next(d)/any(e)/");
    pattern pat = parser.parse(pattern_s);
    normal_pattern given = pattern_normal_form(pat);

    normal_pattern expected;
    expected.prefix.emplace_back(
        pattern_variable("",
          pattern_next(
            pattern_alternation(
              vector<string>({"a"})))));
    expected.prefix.emplace_back(
        pattern_variable("",
          pattern_next(
            pattern_alternation(
              vector<string>({"b"})))));
    expected.groups.emplace_back(
        pattern_any(
          pattern_alternation(
            vector<string>({"c"}))),
        vector<pattern_variable>({
          pattern_variable("x",
              pattern_next(
                pattern_alternation(
                  vector<string>({"d"})))),
          pattern_variable("",
              pattern_next(
                pattern_alternation(
                  vector<string>({"e"}))))}));

    REQUIRE(given == expected);
  }
}
