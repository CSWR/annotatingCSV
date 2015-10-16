/**
 * @file hash_table.cpp
 * Fixed size hash table implementation.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "hash_table.hpp"
#include <cmath>
#include "murmur3.h"
#include <algorithm>
#include <utility>
#include <string>
#include <vector>

using namespace std;

namespace csvp {
namespace navigational {
namespace eval {

constexpr uint32_t Seed = 12345;

table_match::table_match(const span& group_,
    const vector<pair<string, span>>& assignment_)
  : group(group_), assignment(assignment_) {}

bucket::bucket() : curr_index(0) {}

hash_table::hash_table(const string& document, size_t offset, size_t size)
  : document_(document), offset_(offset), data_(size) {}

void hash_table::add(const span& group,
    const vector<pair<string, span>>& assignment) {
  auto hash = hash_span(group);
  data_[hash].matches.emplace_back(group, assignment);
}

hash_table::table_it exponential_search(bucket& bucket, const span& group) {
  auto end = bucket.matches.end();
  auto it = bucket.matches.begin() + bucket.curr_index;
  auto size = distance(it, end);

  if (size == 0) return end;

  long bound = 1;
  while (bound < size && (it + bound)->group.start < group.end) {
    bound *= 2;
  }

  // Binary search
  auto lower = bound/2;
  auto upper = min(bound, size);
  it += lower;
  auto other = it;
  auto count = upper - lower;
  auto step = count/2;
  while (count > 0) {
    other = it; step = count/2; advance(it, step);
    if (other->group.start < group.end) {
      it = ++other;
      count -= step+1;
    } else count = step;
  }

  bucket.curr_index = distance(bucket.matches.begin(), it);
  return it;
}

pair<hash_table::table_it, hash_table::table_it>
hash_table::get(const span& group) {
  auto hash = hash_span(group);
  auto& bucket = data_[hash];
  auto begin = exponential_search(bucket, group);

  return make_pair(begin, bucket.matches.end());
}

uint32_t hash_table::hash_span(const span& span) const {
  const char *key = document_.data() + offset_ + span.start;
  int len = span.length();
  uint32_t result;
  MurmurHash3_x86_32(key, len, Seed, &result);
  return result % data_.size();
}

} // namespace eval
} // namespace navigational
} // namespace csvp
