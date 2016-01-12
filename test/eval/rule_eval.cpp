/*
 * Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "catch.hpp"

#include "eval/rule_eval.hpp"

using namespace std;
using namespace csvp::navigational::eval;
using namespace csvp::navigational::parser;

TEST_CASE("Test rule evaluation", "[rule_eval]") {
  SECTION("test a simple rule") {
    string document("abbcabbc");
    map<string, vector<span>> context;
    rule_eval evaluator(context, document);

    rule simple = {
      predicate({
          "P",
          vector<string>({"x"})
          }),
      vector<clause>({
        pattern({
          vector<pattern_expression>({
            pattern_any(
              pattern_alternation(
                vector<string>({
                  "a"
                })
              )
            ),
            pattern_variable("x",
              pattern_next(
                pattern_alternation(
                  vector<string>({
                    "c"
                  })
                )
              )
            )
          })
        })
      })
    };

    auto answer = evaluator.eval(simple);
    vector<span> expected({span(1,3), span(5,7)});

    REQUIRE(answer == expected);
  }

  SECTION("test a simple rule with equality") {
    string document("abbcabbc");
    map<string, vector<span>> context;
    rule_eval evaluator(context, document);

    rule simple = {
      predicate({
          "P",
          vector<string>({"x"})
          }),
      vector<clause>({
        pattern({
          vector<pattern_expression>({
            pattern_any(
              pattern_alternation(
                vector<string>({
                  "a"
                })
              )
            ),
            pattern_variable("x",
              pattern_next(
                pattern_alternation(
                  vector<string>({
                    "c"
                  })
                )
              )
            ),
            pattern_next(
              pattern_alternation(
                vector<string>({
                  "a"
                })
              )
            ),
            pattern_variable("y",
              pattern_next(
                pattern_alternation(
                  vector<string>({
                    "c"
                  })
                )
              )
            )
          })
        }),
        content_equality({
          false,
          "x",
          "y"
        })
      })
    };

    auto answer = evaluator.eval(simple);
    vector<span> expected({span(1,3)});

    REQUIRE(answer == expected);
  }
}
