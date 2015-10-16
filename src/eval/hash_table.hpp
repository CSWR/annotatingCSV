/**
 * @file hash_table.hpp
 * Fixed size hash table implementation.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <vector>
#include <string>
#include <utility>
#include "span.hpp"

namespace csvp {
namespace navigational {
namespace eval {

struct table_match {
  const span group;
  const std::vector<std::pair<std::string, span>>& assignment;
  table_match(const span& group_,
      const std::vector<std::pair<std::string, span>>& assignment_);
};

struct bucket {
  bucket();
  size_t curr_index;
  std::vector<table_match> matches;
};

class hash_table {
public:
  using table_it = std::vector<table_match>::iterator;
  hash_table(const std::string& document, size_t offset, size_t size);
  void add(const span& group,
      const std::vector<std::pair<std::string, span>>& assignment);
  std::pair<table_it, table_it> get(const span& group);
private:
  uint32_t hash_span(const span& span) const;
  const std::string& document_;
  size_t offset_;
  std::vector<bucket> data_;
};

} // namespace eval
} // namespace navigational
} // namespace csvp

#endif
