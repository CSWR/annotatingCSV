/**
 * @file prefix_step.hpp
 * Matches the pattern prefix to a document.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#ifndef PREFIX_STEP_H
#define PREFIX_STEP_H

#include "pattern_normal_form.hpp"
#include "span.hpp"

namespace csvp {
namespace navigational {
namespace eval {

/**
 * Match a pattern prefix.
 * @param[in]  prefix Pattern prefix.
 * @param[out] result Variable assignment for prefix.
 * @param[in]  begin  Begin iterator of document.
 * @param[in]  end    End iterator of document.
 * @returns iterator where prefix match ends.
 */
std::string::const_iterator prefix_step(
    std::vector<parser::pattern_variable>& prefix,
    std::vector<std::pair<std::string, span>>& result,
    std::string::const_iterator begin,
    std::string::const_iterator end);

} // namespace eval
} // namespace navigational
} // namespace csvp

#endif
