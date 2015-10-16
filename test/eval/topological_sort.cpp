/*
 * Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "catch.hpp"
#include "eval/topological_sort.hpp"

#include <iostream>
using namespace std;
using namespace csvp::navigational::eval;

TEST_CASE("Test topological sort", "[topological_sort]") {
  SECTION("sorts a dag correctly") {
    topological_sort<int,int> sorter;
    int one = 1, two = 2, three = 3, four = 4;

    sorter.add_node(one, one);
    sorter.add_node(two, two);
    sorter.add_node(three, three);
    sorter.add_node(four, four);

    sorter.add_edge(one, two);
    sorter.add_edge(one, three);
    sorter.add_edge(three, four);
    sorter.add_edge(two, four);
    sorter.add_edge(two, three);

    vector<int*> given;
    bool is_a_dag = sorter.sort(given);

    vector<int*> expected({&one,&two,&three,&four});

    REQUIRE(is_a_dag);
    REQUIRE(given == expected);
  }

  SECTION("sorts a dag correctly") {
    topological_sort<int, int> sorter;
    int one = 1, two = 2, three = 3, four = 4;
    int other = 2;

    sorter.add_node(one, one);
    sorter.add_node(two, two);
    sorter.add_node(three, three);
    sorter.add_node(four, four);
    sorter.add_node(other, other);

    sorter.add_edge(one, two);
    sorter.add_edge(one, three);
    sorter.add_edge(three, four);
    sorter.add_edge(two, four);
    sorter.add_edge(two, three);

    vector<int*> given;
    bool is_a_dag = sorter.sort(given);

    vector<int*> expected({&one,&other,&two,&three,&four});

    REQUIRE(is_a_dag);
    REQUIRE(given == expected);
  }

  SECTION("detects cycles") {
    topological_sort<int, int> sorter;
    int one = 1, two = 2, three = 3, four = 4;

    sorter.add_node(one, one);
    sorter.add_node(two, two);
    sorter.add_node(three, three);
    sorter.add_node(four, four);

    sorter.add_edge(one, two);
    sorter.add_edge(two, three);
    sorter.add_edge(three, four);
    sorter.add_edge(four, one);

    vector<int*> given;
    bool is_a_dag = sorter.sort(given);

    REQUIRE(!is_a_dag);
  }
}
