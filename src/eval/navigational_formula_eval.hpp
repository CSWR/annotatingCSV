/**
 * @file navigational_formula_eval.hpp
 * Evaluates navigational formulas.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#ifndef NAVIGATIONAL_FORMULA_EVAL_H
#define NAVIGATIONAL_FORMULA_EVAL_H

#include "../parser/navigational_parser_types.hpp"
#include "pattern_normal_form.hpp"
#include "step4.hpp"

namespace csvp {
namespace navigational {
namespace eval {

/**
 * Evaluate navigational formula over document.
 * @param pattern           Navigational pattern.
 * @param document          Target document.
 * @param prefix_assignment Assignment for prefix.
 * @param benchmark         Wether to print benchmark times.
 * @returns set of assignments over document.
 */
assignment_set navigational_formula_eval(parser::pattern& pattern,
    const std::string& document,
    std::vector<std::pair<std::string, span>>& prefix_assignment,
    bool benchmark = false);

} // namespace eval
} // namespace navigational
} // namespace csvp

#endif
