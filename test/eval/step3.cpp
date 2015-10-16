/*
 * Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "catch.hpp"

#include "eval/step3.hpp"

using namespace std;
using namespace csvp::navigational::eval;

TEST_CASE("Test step three", "[step3]") {
  SECTION("the R array is correctly filled") {
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
          make_pair("x", span(1,3))}));
    auto& m3 = group2.matches.back();
    group2.matches.emplace_back(4,8,
        vector<pair<string, span>>({
          make_pair("x", span(5,7))}));
    auto& m4 = group2.matches.back();
    group2.next_match = vector<c_match*>({&m3,&m4,&m4,&m4,&m4,0,0,0,0});

    auto result = step3(c_matches, document.begin(), document.end());

    vector<size_t> expected({0,1,1,1,1,2,2,2,2});

    REQUIRE(expected == result);
  }
}
