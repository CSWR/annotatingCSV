/*
 * Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "catch.hpp"

#include "eval/step2.hpp"

using namespace std;
using namespace csvp::navigational::eval;

TEST_CASE("Test step two", "[step2]") {
  SECTION("the C array is correctly filled") {
    string document("abbcabbc");
    group_matches group_m;

    // any(a)
    group_m.any = make_shared<next_matches>();
    group_m.any->matches.emplace_back(0,1);
    group_m.any->matches.emplace_back(4,5);
    span *m1 = &group_m.any->matches[0];
    span *m2 = &group_m.any->matches[1];
    group_m.any->next_match = vector<span*>({m1,m2,m2,m2,m2,0,0,0,0});
    group_m.any->filled = true;

    // ?x:next(c)
    group_m.nexts.emplace_back("x", make_shared<next_matches>());
    next_matches& next_m = *group_m.nexts[0].next_matches;
    next_m.matches.emplace_back(3,4);
    next_m.matches.emplace_back(7,8);
    m1 = &next_m.matches[0];
    m2 = &next_m.matches[1];
    next_m.next_match = vector<span*>({m1,m1,m1,m1,m2,m2,m2,m2,0});
    next_m.filled = true;

    auto result = step2_single(group_m, document.begin(), document.end());

    REQUIRE(result.matches.size() == 2);

    c_match match1(0, 4,
      vector<pair<string, span>>({
        make_pair("x", span(1, 3))}));
    c_match match2(4, 8,
      vector<pair<string, span>>({
        make_pair("x", span(5, 7))}));

    list<c_match> e_matches({match1, match2});

    REQUIRE(e_matches == result.matches);

    auto& next_match = result.next_match;

    REQUIRE(*next_match[0] == match1);
    REQUIRE(*next_match[1] == match2);
    REQUIRE(*next_match[2] == match2);
    REQUIRE(*next_match[3] == match2);
    REQUIRE(*next_match[4] == match2);
    REQUIRE(next_match[5] == nullptr);
    REQUIRE(next_match[6] == nullptr);
    REQUIRE(next_match[7] == nullptr);
    REQUIRE(next_match[8] == nullptr);
  }
}
