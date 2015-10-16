/**
 * @file pattern_normal_form.cpp
 * Converts a pattern into normal form.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "pattern_normal_form.hpp"
#include <algorithm>

using namespace std;

namespace csvp {
namespace navigational {
namespace eval {

using namespace parser;

/**
 * Visitor to transverse pattern.
 */
class normal_visitor : public boost::static_visitor<> {
public:
  normal_visitor(normal_pattern& normal) : normal_(normal) {}

  void visit(pattern& pattern) {
    // Iterate in reverse
    auto& expressions = pattern.expressions;

    for (auto it = expressions.rbegin(); it != expressions.rend(); it++) {
      boost::apply_visitor(*this, *it);
    }

    // Last group or prefix
    normal_.prefix.reserve(next_group_.size());
    normal_.prefix.insert(normal_.prefix.end(),
        next_group_.rbegin(), next_group_.rend());
  }

  void operator()(pattern_next& next) {
    // Sort
    auto& words = next.alternation.words;
    sort(words.begin(), words.end());

    // Add to next group
    for (int i = 0; i < next.count; i++) {
      string empty = "";
      next_group_.emplace_back(empty, next);
    }
  }

  void operator()(pattern_any& any) {
    // Sort
    auto& words = any.alternation.words;
    sort(words.begin(), words.end());

    if (!var_seen_) {
      // If a variable hasn't been seen, we can transform to next
      string empty = "";
      next_group_.emplace_back(empty, any.alternation);
    }
    else {
      // We "close" the group and start a new one
      normal_.groups.emplace_back();
      pattern_group& group = normal_.groups.back();
      group.any = any;
      group.nexts.reserve(next_group_.size());
      group.nexts.insert(group.nexts.end(),
          make_move_iterator(next_group_.rbegin()),
          make_move_iterator(next_group_.rend()));
      next_group_.clear();
      var_seen_ = false;
    }
  }

  void operator()(pattern_variable& variable) {
    // Sort
    auto& words = variable.next.alternation.words;
    sort(words.begin(), words.end());

    // Mark seen and add to next group
    var_seen_ = true;
    next_group_.emplace_back(variable);
  }

private:
  vector<pattern_variable> next_group_;
  bool var_seen_ = false;
  normal_pattern& normal_;
};

bool pattern_group::operator==(const pattern_group& rhs) const {
  return any == rhs.any && nexts == rhs.nexts;
}

bool normal_pattern::operator==(const normal_pattern& rhs) const {
  return prefix == rhs.prefix && groups == rhs.groups;
}

normal_pattern pattern_normal_form(parser::pattern& pattern) {
  normal_pattern normal;
  normal_visitor visitor(normal);

  visitor.visit(pattern);

  // Reverse groups
  reverse(normal.groups.begin(), normal.groups.end());
  return normal;
}

} // namespace eval
} // namespace navigational
} // namespace csvp
