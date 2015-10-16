/**
 * @file assignment_set.cpp
 * Holds a set of assignments in compact form.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "assignment_set.hpp"
#include <utility>
#include <string>
#include <vector>

using namespace std;

namespace csvp {
namespace navigational {
namespace eval {

assignment_node::assignment_node(assignment_node *parent_)
  : parent(parent_) {}

assignment_set::assignment_set(size_t size) :
  root_(nullptr), current_(nullptr), current_depth_(0), path_(size + 1),
  current_child_(size + 1) {}

void add_offset(vector<pair<string, span>>& assignment, size_t offset) {
  for (auto& pair : assignment) {
    pair.second.start += offset;
    pair.second.end += offset;
  }
}

assignment_node *assignment_set::move_up(assignment_node *current) {
  if (!current->parent) return current;
  return current->parent;
}

assignment_node *assignment_set::add_child(assignment_node *current,
    const vector<pair<string, span>>& assignment, size_t offset) {
  current->children.emplace_back(new assignment_node(current));
  current = current->children.back().get();
  current->assignment = assignment;
  add_offset(current->assignment, offset);
  return current;
}

bool assignment_set::next(vector<pair<string, span>*>& answer) {
  // First value
  if (!current_) {
    current_ = &root_;
    path_[0] = current_;
    current_depth_ = 0;
  }

  while (current_depth_ != -1 && !current_->children.empty()) {
    if (current_child_[current_depth_] >= current_->children.size()) {
      // All children have been visited. Go up.
      current_child_[current_depth_] = 0;
      current_depth_--;
      if (current_depth_ >= 0)
        current_ = path_[current_depth_];
    } else {
      // Visit next child. Go down.
      current_ = current_->children[current_child_[current_depth_]].get();
      current_child_[current_depth_]++;
      current_depth_++;
      path_[current_depth_] = current_;
    }
  }

  if (current_depth_ != -1 && current_ != &root_) {
    // Return answer
    answer.clear();
    for (auto it = std::next(path_.begin()); it != path_.end(); it++)
      for (auto& assign : (*it)->assignment)
        answer.emplace_back(&assign);
    // Go up
    current_depth_--;
    current_ = path_[current_depth_];
    return true;
  } else {
    // Empty answer
    current_ = nullptr;
    return false;
  }
}

size_t count_leaves(const assignment_node& node) {
  if (node.children.empty()) return 1;
  size_t sum = 0;
  for (auto& child : node.children)
    sum += count_leaves(*child);
  return sum;
}

size_t assignment_set::size() const {
  if (root_.children.empty()) return 0;
  return count_leaves(root_);
}

} // namespace eval
} // namespace navigational
} // namespace csvp
