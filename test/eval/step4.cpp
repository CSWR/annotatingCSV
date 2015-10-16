/*
 * Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "catch.hpp"

#include "eval/step4.hpp"

using namespace std;
using namespace csvp::navigational::eval;

TEST_CASE("Test step four", "[step4]") {
  SECTION("the correct answers are found") {
    string document("abbcabbc");

    vector<group_c_match> c_matches;

    c_matches.emplace_back(document.size() + 1);
    auto& group1 = c_matches.back();
    group1.matches.emplace_back(0,4,
        vector<pair<string, span>>({
          make_pair("x", span(1,3))}));
    auto& m1 = group1.matches.back();
    group1.matches.emplace_back(4,8,
        vector<pair<string, span>>({
          make_pair("x", span(5,7))}));
    auto& m2 = group1.matches.back();
    group1.next_match = vector<c_match*>({&m1,&m2,&m2,&m2,&m2,0,0,0,0});

    c_matches.emplace_back(document.size() + 1);
    auto& group2 = c_matches.back();
    group2.matches.emplace_back(0,4,
        vector<pair<string, span>>({
          make_pair("y", span(1,3))}));
    auto& m3 = group2.matches.back();
    group2.matches.emplace_back(4,8,
        vector<pair<string, span>>({
          make_pair("y", span(5,7))}));
    auto& m4 = group2.matches.back();
    group2.next_match = vector<c_match*>({&m3,&m4,&m4,&m4,&m4,0,0,0,0});

    vector<size_t> R({0,1,1,1,1,2,2,2,2});

    auto result = step4(c_matches, R, 0);

    vector<pair<string, span>*> answer;
    bool success = result.next(answer);

    REQUIRE(success);
    REQUIRE(answer[0]->first == "x");
    REQUIRE(answer[0]->second == span(1,3));
    REQUIRE(answer[1]->first == "y");
    REQUIRE(answer[1]->second == span(5,7));
    REQUIRE(!result.next(answer));
  }
}
