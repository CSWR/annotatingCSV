/**
 * @file span.cpp
 * Represents a span in a document.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "span.hpp"

using namespace std;

namespace csvp {
namespace navigational {
namespace eval {

span::span() : start(0), end(0) {}

span::span(size_t start_, size_t end_)
  : start(start_), end(end_) {}

size_t span::length() const {
  return end - start;
}

bool span::operator<(const span& rhs) const {
  if (start == rhs.start) return end < rhs.end;
  return start < rhs.start;
}

bool span::operator==(const span& rhs) const {
  return start == rhs.start && end == rhs.end;
}

bool span::operator!=(const span& rhs) const {
  return !operator==(rhs);
}

ostream& operator<<(ostream& out, span& match) {
  return out << '(' << match.start << ',' << match.end << ')';
}

bool span_content_eq(const span& lhs, const span& rhs,
    const std::string& document, size_t offset) {
  size_t lhs_len = lhs.length();
  size_t rhs_len = rhs.length();

  if (lhs_len != rhs_len) return false;
  return !document.compare(lhs.start + offset, lhs_len,
                 document, rhs.start + offset, rhs_len);
}

} // namespace eval
} // namespace navigational
} // namespace csvp
