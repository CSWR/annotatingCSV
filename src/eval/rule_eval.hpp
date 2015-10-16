/**
 * @file rule_eval.hpp
 * Evaluate datalog rule.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#ifndef RULE_EVAL_H
#define RULE_EVAL_H

#include "../parser/datalog_parser_types.hpp"
#include "step4.hpp"
#include <map>
#include <memory>

namespace csvp {
namespace navigational {
namespace eval {

/**
 * Represents a table of assignments, where each row is an assignment and each
 * column is a variable.
 */
class assignment_table {
public:
  assignment_table(std::vector<std::pair<std::string, span>>& prefix,
      assignment_set& assignments);
  /**
   * Projects the table into a variable.
   * @param[in]  variable Variable to project to.
   * @param[out] spans    Projected spans.
   * @returns false if the table doesn't have the variable.
   */
  bool project(const std::string& variable, std::vector<span>& spans) const;
  const std::map<std::string, size_t>& variable_index() const
  { return variable_index_; }
  std::vector<std::vector<span>>& assignments() { return assignments_; }
private:
  std::vector<std::vector<span>> assignments_; /**< Actual table. */
  std::map<std::string, size_t> variable_index_; /**< Map from var to column. */
};

class rule_eval : public boost::static_visitor<void> {
public:
  rule_eval(const std::map<std::string, std::vector<span>>& context,
      const std::string& document);
  std::vector<span> eval(parser::rule& rule);

  void operator()(parser::predicate& predicate);
  void operator()(parser::pattern& pattern);
  void operator()(parser::span_equality& span_eq);
  void operator()(parser::content_equality& content_eq);
  void operator()(parser::filter& filter);
private:
  const std::map<std::string, std::vector<span>>& context_;
  const std::string& document_;
  std::list<std::unique_ptr<assignment_table>> tables_;
};

} // namespace eval
} // namespace navigational
} // namespace csvp

#endif
