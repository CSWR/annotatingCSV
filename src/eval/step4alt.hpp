/**
 * @file step4alt.hpp
 * Alternate step4 implementation with support for backrefences.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#ifndef STEP4ALT_H
#define STEP4ALT_H

#include "step2.hpp"
#include "assignment_set.hpp"

namespace csvp {
namespace navigational {
namespace eval {

assignment_set step4alt(std::vector<group_c_match>& C, std::vector<size_t> R,
    size_t offset, const std::string& document);

} // namespace eval
} // namespace navigational
} // namespace csvp

#endif
