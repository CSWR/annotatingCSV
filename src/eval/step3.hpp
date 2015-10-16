/**
 * @file step3.hpp
 * Step three in the core algorithm.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#ifndef STEP3_H
#define STEP3_H

#include "step2.hpp"

namespace csvp {
namespace navigational {
namespace eval {

std::vector<size_t> step3(std::vector<group_c_match>& pattern_c_match,
    std::string::const_iterator begin, std::string::const_iterator end);

} // namespace eval
} // namespace navigational
} // namespace csvp

#endif
