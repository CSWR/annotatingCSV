/*
 * Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "catch.hpp"

#include "parser/datalog_parser.hpp"
#include <string>

using namespace csvp::navigational::parser;
using namespace std;

TEST_CASE("the datalog paser works correctly", "[datalog_parser]") {
  datalog_parser parser;
  program prog;

  SECTION("Test with example input") {
    string program_s("# comment\n"
                     "P(?x) :- /?x:next(,|\\n)/, not &x = &y;\n"
                     "P(?x) :- /any(,|\\n)/?x:next(,|\\n)/, not ?x.\"abc\";\n");
    prog = parser.parse(program_s);

    program prog2;
    pattern pat1, pat2;
    pat1.expressions.emplace_back(
        pattern_variable("x",
          pattern_next(
            pattern_alternation(
              vector<string>({",","\n"})))));
    pat2.expressions.emplace_back(
        pattern_any(
          pattern_alternation(
            vector<string>({",","\n"}))));
    pat2.expressions.emplace_back(
        pattern_variable("x",
          pattern_next(
            pattern_alternation(
              vector<string>({",","\n"})))));
    prog2.rules.emplace_back();
    prog2.rules[0].pred.name = "P";
    prog2.rules[0].pred.variable = "x";
    prog2.rules[0].clauses.emplace_back(pat1);
    prog2.rules[0].clauses.emplace_back(
        content_equality({true, "x", "y"}));
    prog2.rules.emplace_back();
    prog2.rules[1].pred.name = "P";
    prog2.rules[1].pred.variable = "x";
    prog2.rules[1].clauses.emplace_back(pat2);
    prog2.rules[1].clauses.emplace_back(
        filter({true, "x", "abc"}));

    REQUIRE(prog == prog2);
  }
}
