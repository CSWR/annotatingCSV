/**
 * @file assignment_set.hpp
 * Holds a set of assignments in compact form.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#ifndef ASSIGNMENT_SET_H
#define ASSIGNMENT_SET_H

#include <vector>
#include <memory>
#include <string>
#include <utility>
#include "span.hpp"

namespace csvp {
namespace navigational {
namespace eval {

struct assignment_node {
  std::vector<std::pair<std::string, span>> assignment;
  std::vector<std::unique_ptr<assignment_node>> children;
  assignment_node *parent;

  explicit assignment_node(assignment_node *parent_);
};

/**
 * Represents the set of all assignments for variables.
 * The set of matches for a pattern can be represented as a forest where each
 * node holds assignments for a variable mentioned in a group.
 */
class assignment_set {
public:
  /**
   * Initialize forest.
   * @param size Height of the tree.
   */
  explicit assignment_set(size_t size);
  /**
   * Yield next assignment for variables in the set. Corresponds to a branch in
   * the forest.
   * @param answer Vector to hold the assignment.
   * @returns wether the previous call was the last assignment.
   */
  bool next(std::vector<std::pair<std::string, span>*>& answer);
  static assignment_node *move_up(assignment_node *current);
  static assignment_node *add_child(assignment_node *current,
      const std::vector<std::pair<std::string, span>>& assignment,
      size_t offset);
  /**
   * Calculates the number of assignments or leaves in the forest.
   */
  size_t size() const;
  assignment_node *root() { return &root_; }

private:
  assignment_node root_;               /**< "Root" of the forest. Parent of all
                                            trees. */
  assignment_node *current_;           /**< Current node being explored. */
  int current_depth_;                  /**< Current depth of the tree being
                                            explored. */
  std::vector<assignment_node*> path_; /**< Current path in the tree. */
  std::vector<size_t> current_child_;  /**< Current child being explored for
                                            each node in the path. */
};


} // namespace eval
} // namespace navigational
} // namespace csvp

#endif
