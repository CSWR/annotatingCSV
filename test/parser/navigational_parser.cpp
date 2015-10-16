/*
 * Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "catch.hpp"

#include "parser/navigational_parser.hpp"
#include "parser/navigational_generator.hpp"
#include <string>

using namespace csvp::navigational::parser;
using namespace std;

TEST_CASE("the navigational parser works correctly", "[navigational_parser]") {
  navigational_parser parser;
  navigational_generator generator;
  pattern pat;

  SECTION("Test with example input") {
    string pattern_s("/any(abc|def)/next(a|b)/?x:next(c|d)/");
    pat = parser.parse(pattern_s);

    pattern pat2;
    pat2.expressions.emplace_back(
        pattern_any(
          pattern_alternation(
            vector<string>({"abc","def"}))));
    pat2.expressions.emplace_back(
        pattern_next(
          pattern_alternation(
            vector<string>({"a","b"}))));
    pat2.expressions.emplace_back(pattern_variable("x",
          pattern_next(
            pattern_alternation(
              vector<string>({"c","d"})))));

    REQUIRE(pat == pat2);

    string pattern_out(generator.generate(pat2));

    REQUIRE(pattern_s == pattern_out);
  }

  SECTION("Test special characters") {
    string pattern_s("/any(a\\r\\n|c\\r\\n)/next(a\\|b|c\\|d)/");
    pat = parser.parse(pattern_s);

    pattern pat2;
    pat2.expressions.emplace_back(
        pattern_any(
          pattern_alternation(
            vector<string>({"a\r\n","c\r\n"}))));
    pat2.expressions.emplace_back(
        pattern_next(
          pattern_alternation(
            vector<string>({"a|b","c|d"}))));

    REQUIRE(pat == pat2);

    string pattern_out(generator.generate(pat2));

    REQUIRE(pattern_s == pattern_out);
  }
}
