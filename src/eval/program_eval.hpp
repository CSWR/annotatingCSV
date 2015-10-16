/**
 * @file program_eval.hpp
 * Evaluates a datalog program.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#ifndef PROGRAM_EVAL_H
#define PROGRAM_EVAL_H

#include "span.hpp"
#include "../parser/datalog_parser_types.hpp"
#include <map>
#include <vector>

namespace csvp {
namespace navigational {
namespace eval {

class program_eval {
public:
  program_eval(const std::string& document);
  std::map<std::string, std::vector<span>> eval(parser::program& prog);
private:
  const std::string& document_;

  std::vector<parser::rule*> sort_rules(std::vector<parser::rule>& rules);
};

} // namespace eval
} // namespace navigational
} // namespace csvp

#endif
