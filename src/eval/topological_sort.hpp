/**
 * @file topological_sort.hpp
 * Implementation of topological sort for rule dependency.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#ifndef TOPOLOGICAL_SORT_H
#define TOPOLOGICAL_SORT_H

#include <list>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <cassert>

namespace csvp {
namespace navigational {
namespace eval {

template <class K, class V>
class topological_sort {
private:
  enum struct Mark {
    NONE,
    TEMPORARY,
    PERMANENT
  };

  struct graph_node {
    std::vector<V*> content;
    std::vector<graph_node*> edges;
    Mark mark;

    graph_node(V& elem);
  };

  /**
   * Node visit procedure.
   * @param current_node Node being visited.
   * @param order        Current topological order (in reverse).
   * @returns false if the graph is not a dag.
   */
  bool visit(graph_node& current_node, std::vector<V*>& order);

  std::list<graph_node> nodes_; /**< List of all nodes. */
  std::unordered_map<K, graph_node*> node_map_; /**< Map of nodes by value. */

public:
  void add_node(K& key, V& value);
  void add_edge(K& from, K& to);
  bool sort(std::vector<V*>& order);
};

template <class K, class V>
topological_sort<K,V>::graph_node::graph_node(V& elem)
: mark(Mark::NONE) {
  content.push_back(&elem);
}

template <class K, class V>
void topological_sort<K,V>::add_node(K& key, V& value) {
  auto it = node_map_.find(key);

  if (it == node_map_.end()) {
    nodes_.emplace_back(value);
    node_map_[key] = &nodes_.back();
  }
  else {
    it->second->content.push_back(&value);
  }
}

template <class K, class V>
void topological_sort<K,V>::add_edge(K& from, K& to) {
  auto from_node = node_map_[from];
  auto to_node = node_map_[to];
  from_node->edges.push_back(to_node);
}

template <class K, class V>
bool topological_sort<K,V>::visit(graph_node& node, std::vector<V*>& order) {
  switch(node.mark) {
    case Mark::NONE:
      node.mark = Mark::TEMPORARY;

      // Visit all children
      for (auto& child : node.edges) {
        bool is_a_dag = visit(*child, order);
        if (!is_a_dag) return false;
      }

      // Add elements to the order
      node.mark = Mark::PERMANENT;
      order.insert(order.end(), node.content.begin(), node.content.end());
    break;
    case Mark::TEMPORARY:
      // Not a dag
      return false;
    break;
    case Mark::PERMANENT:
      // Nothing to do.
    break;
  }

  return true;
}

template <class K, class V>
bool topological_sort<K,V>::sort(std::vector<V*>& order) {
  assert(order.empty());
  for (auto& node : nodes_) {
    bool is_a_dag = visit(node, order);
    if (!is_a_dag) return false;
  }

  std::reverse(order.begin(), order.end());
  return true;
}

} // namespace eval
} // namespace navigational
} // namespace csvp

#endif
