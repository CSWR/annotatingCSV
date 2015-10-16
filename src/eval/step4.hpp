/**
 * @file step4.hpp
 * Step four of the core algorithm.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#ifndef STEP4_H
#define STEP4_H

#include "step2.hpp"
#include "assignment_set.hpp"

namespace csvp {
namespace navigational {
namespace eval {

assignment_set step4(std::vector<group_c_match>& C, std::vector<size_t> R,
    size_t offset);

} // namespace eval
} // namespace navigational
} // namespace csvp

#endif
