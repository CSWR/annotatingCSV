/**
 * @file rule_eval.cpp
 * Evaluate datalog rule.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "rule_eval.hpp"
#include "navigational_formula_eval.hpp"
#include "re2/re2.h"
#include <cassert>

using namespace std;
using namespace re2;

namespace csvp {
namespace navigational {
namespace eval {

assignment_table::assignment_table(vector<pair<string, span>>& prefix,
    assignment_set& set) {
  // Map prefix variables
  for (size_t i = 0; i < prefix.size(); i++) {
    variable_index_[prefix[i].first] = i;
  }

  size_t num_of_assignments = set.size();
  if (num_of_assignments == 0) {

    // Just prefix
    assignments_.emplace_back();
    auto& assign = assignments_.back();
    assign.reserve(prefix.size());
    for (auto& pair : prefix)
      assign.emplace_back(pair.second);

    return;
  }

  size_t assign_count = 0;

  // First assignment
  vector<pair<string, span>*> assignment;
  set.next(assignment);

  // Allocate memory
  size_t assignment_length = prefix.size() + assignment.size();
  assignments_.resize(num_of_assignments, vector<span>(assignment_length));

  // Map variables
  for (size_t i = 0; i < assignment.size(); i++) {
    variable_index_[assignment[i]->first] = i;
  }

  do {
    // Fill prefix
    for (size_t i = 0; i < prefix.size(); i++) {
      assignments_[assign_count][i] = prefix[i].second;
    }

    // Fill the rest
    for (size_t i = 0; i < assignment.size(); i++) {
      size_t offset = prefix.size() + i;
      assignments_[assign_count][offset] = assignment[i]->second;
    }

    assign_count++;
  } while (set.next(assignment));
}

bool assignment_table::project(const string& variable,
    vector<span>& spans) const {
  assert(spans.empty());

  auto it = variable_index_.find(variable);

  if (it == variable_index_.end()) return false;

  auto var_idx = it->second;
  spans.reserve(assignments_.size());

  for (auto& assignment : assignments_) {
    spans.push_back(assignment[var_idx]);
  }

  return true;
}

rule_eval::rule_eval(const map<string, vector<span>>& context,
    const string& document) : context_(context), document_(document) {}

vector<span> rule_eval::eval(parser::rule& rule) {
  vector<span> answer;

  for (auto& clause : rule.clauses) {
    boost::apply_visitor(*this, clause);
  }

  // If not unary, do nothing.
  if (rule.pred.variables.size() > 1) return vector<span>();

  for (auto& table : tables_) {
    if (table->project(rule.pred.variables[0], answer))
      break;
  }

  return answer;
}

void rule_eval::operator()(parser::predicate& predicate) {
  // Verify that predicate is unary
  if (predicate.variables.size() > 1) {
    throw std::runtime_error("Error: Only unary predicates are supported.");
  }

  // Filter each table
  for (auto& table : tables_) {
    auto& index = table->variable_index();
    auto it = index.find(predicate.variables[0]);

    // Not found, continue
    if (it == index.end()) continue;

    size_t idx = it->second;
    /* bool negated = predicate.negated; */

    auto& assignments = table->assignments();

    // Remove those that don't satisfy the predicate
    auto new_end = remove_if(assignments.begin(), assignments.end(),
        [idx, predicate, this]
        (const vector<span>& assignment) {
          auto& span = assignment[idx];
          auto it = context_.find(predicate.name);
          auto& spans = it->second;

          auto found = binary_search(spans.begin(), spans.end(), span);
          return !found;
        });
    assignments.erase(new_end, assignments.end());
  }
}

void rule_eval::operator()(parser::pattern& pattern) {
  // Evaluate pattern
  vector<pair<string, span>> prefix;
  auto set = navigational_formula_eval(pattern, document_, prefix);

  // Materialize table
  tables_.emplace_back(new assignment_table(prefix, set));
}

void rule_eval::operator()(parser::span_equality& span_eq) {
  // TODO(francisco) Implement span equality in the future
}

void rule_eval::operator()(parser::content_equality& content_eq) {
  for (auto& table : tables_) {
    auto& index = table->variable_index();
    auto first_it = index.find(content_eq.first_var);
    auto second_it = index.find(content_eq.second_var);

    // Just handling the case where both vars are in the same table
    if (first_it == index.end() || second_it == index.end())
      continue;

    size_t first_idx = first_it->second;
    size_t second_idx = second_it->second;
    bool negated = content_eq.negated;

    auto& assignments = table->assignments();

    // Remove those that don't satisfy the equality
    auto new_end = remove_if(assignments.begin(), assignments.end(),
        [first_idx, second_idx, negated, this]
        (const vector<span>& assignment) {
          auto& first = assignment[first_idx];
          auto& second = assignment[second_idx];
          bool is_equal = span_content_eq(first, second, document_);
          return negated ? is_equal : !is_equal;
        });
    assignments.erase(new_end, assignments.end());
  }
}

void rule_eval::operator()(parser::filter& filter) {
  // Compile regex
  RE2 re(filter.regex);
  if (!re.ok()) {
    throw runtime_error(re.error());
  }

  for (auto& table : tables_) {
    auto& index = table->variable_index();
    auto it = index.find(filter.variable);

    // Not found, continue
    if (it == index.end()) continue;

    size_t idx = it->second;
    bool negated = filter.negated;

    auto& assignments = table->assignments();

    // Remove those that don't satisfy the filter
    auto new_end = remove_if(assignments.begin(), assignments.end(),
        [idx, negated, &re, this]
        (const vector<span>& assignment) {
          auto& span = assignment[idx];
          StringPiece input(document_.data() + span.start, span.length());
          bool matches = RE2::FullMatch(input, re);
          return negated ? matches : !matches;
        });
    assignments.erase(new_end, assignments.end());
  }
}

} // namespace eval
} // namespace navigational
} // namespace csvp
