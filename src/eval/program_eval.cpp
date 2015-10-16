/**
 * @file program_eval.cpp
 * Evaluates a datalog program.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "program_eval.hpp"
#include "topological_sort.hpp"
#include "rule_eval.hpp"
#include <utility>

using namespace std;
using namespace csvp::navigational::parser;

namespace csvp {
namespace navigational {
namespace eval {

program_eval::program_eval(const std::string& document) : document_(document) {}

map<string, vector<span>> program_eval::eval(program& prog) {
  map<string, vector<span>> answer;
  auto sorted_rules = sort_rules(prog.rules);

  for (auto rule : sorted_rules) {
    rule_eval evaluator(answer, document_);
    auto result = evaluator.eval(*rule);

    auto it = answer.find(rule->pred.name);

    if (it == answer.end()) {
      // Move
      answer.insert(make_pair(rule->pred.name, move(result)));
    } else {
      // Union
      auto& spans = it->second;
      vector<span> span_union;
      span_union.reserve(spans.size() + result.size());
      set_union(spans.begin(), spans.end(), result.begin(), result.end(),
          back_inserter(span_union));
      span_union.shrink_to_fit();
      spans.swap(span_union);
    }
  }

  return answer;
}

class edge_adder_visitor : public boost::static_visitor<void> {
public:
  edge_adder_visitor(rule& current, topological_sort<string, rule>& sorter)
    : current_(current), sorter_(sorter) {}
  void operator()(predicate& predicate) {
    sorter_.add_edge(predicate.name, current_.pred.name);
  }
  void operator()(pattern&) {}
  void operator()(span_equality&) {}
  void operator()(content_equality&) {}
  void operator()(filter&) {}
private:
  rule& current_;
  topological_sort<string, rule>& sorter_;
};

vector<rule*> program_eval::sort_rules(vector<rule>& rules) {
  vector<rule*> ordered;
  topological_sort<string, rule> sorter;

  for (auto& rule : rules) {
    sorter.add_node(rule.pred.name, rule);
  }

  for (auto& rule : rules) {
    edge_adder_visitor visitor(rule, sorter);
    for (auto& clause : rule.clauses)
      boost::apply_visitor(visitor, clause);
  }

  bool success = sorter.sort(ordered);

  if (!success) throw runtime_error("Rule reference cycle found.");

  return ordered;
}

} // namespace eval
} // namespace navigational
} // namespace csvp
