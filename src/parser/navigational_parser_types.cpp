/**
 * @file navigational_parser_types.cpp
 * Types for representing a program in the navigational language.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */

#include "navigational_parser_types.hpp"

using namespace std;

namespace csvp {
namespace navigational {
namespace parser {

class var_name_visitor : public boost::static_visitor<void> {
public:
  var_name_visitor(vector<string>& target) : target_(target) {}
  void operator()(const pattern_next&) {}
  void operator()(const pattern_any&) {}
  void operator()(const pattern_variable& variable) {
    target_.emplace_back(variable.variable);
  }
private:
  vector<string>& target_;
};

struct backreference_visitor : public boost::static_visitor<bool> {
  bool operator()(const pattern_next&) const { return false; }
  bool operator()(const pattern_any&) const { return false; }
  bool operator()(const pattern_variable& variable) const {
    return variable.content;
  }
};

vector<string> pattern::var_names() const {
  vector<string> ret;
  var_name_visitor visitor(ret);
  for (auto& expression : expressions) {
    boost::apply_visitor(visitor, expression);
  }
  return ret;
}

bool pattern::has_backreference() const {
  backreference_visitor visitor;
  for (auto& expression : expressions) {
    if (boost::apply_visitor(visitor, expression))
      return true;
  }
  return false;
}

bool pattern::operator==(const pattern& rhs) const {
  return expressions == rhs.expressions;
}

bool pattern_alternation::operator==(const pattern_alternation& rhs) const {
  return words == rhs.words;
}

bool pattern_next::operator==(const pattern_next& rhs) const {
  return count == rhs.count && alternation == rhs.alternation;
}

bool pattern_any::operator==(const pattern_any& rhs) const {
  return alternation == rhs.alternation;
}

bool pattern_variable::operator==(const pattern_variable& rhs) const {
  return content == rhs.content && variable == rhs.variable
    && next == rhs.next;
}

} // namespace parser
} // namespace navigational
} // namespace csvp
