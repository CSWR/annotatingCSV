/**
 * @file datalog_parser_types.cpp
 * Types for representing a program in modified datalog.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "datalog_parser_types.hpp"

using namespace std;

namespace csvp {
namespace navigational {
namespace parser {

bool program::operator==(const program& rhs) const {
  return rules == rhs.rules;
}

bool predicate::operator==(const predicate& rhs) const {
  return name == rhs.name && variable == rhs.variable;
}

bool span_equality::operator==(const span_equality& rhs) const {
  return first_var == rhs.first_var && second_var == rhs.second_var
    && negated == rhs.negated;
}

bool content_equality::operator==(const content_equality& rhs) const {
  return first_var == rhs.first_var && second_var == rhs.second_var
    && negated == rhs.negated;
}

bool filter::operator==(const filter& rhs) const {
  return variable == rhs.variable && regex == rhs.regex
    && negated == rhs.negated;
}

bool rule::operator==(const rule& rhs) const {
  return pred == rhs.pred && clauses == rhs.clauses;
}

} // namespace parser
} // namespace navigational
} // namepace csvp
