/**
 * @file span.hpp
 * Represents a span in a document.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#ifndef SPAN_H
#define SPAN_H

#include <ostream>

namespace csvp {
namespace navigational {
namespace eval {

struct span {
  size_t start; /**< Start position of the match. */
  size_t end;   /**< End position of the match (exclusive). */

  span();
  span(size_t start_, size_t end_);
  span(const span&) = default;
  span& operator=(const span&) = default;
  size_t length() const;
  bool operator<(const span& rhs) const;
  bool operator==(const span& rhs) const;
  bool operator!=(const span& rhs) const;
};

std::ostream& operator<<(std::ostream& out, span& match);

/**
 * Checks two spans for content equality.
 * @param lhs,rhs  Spans to compare.
 * @param document Document to which the spans belong.
 * @param offset   Span offset in the document.
 */
bool span_content_eq(const span& lhs, const span& rhs,
    const std::string& document, size_t offset = 0);

} // namespace eval
} // namespace navigational
} // namespace csvp

#endif
