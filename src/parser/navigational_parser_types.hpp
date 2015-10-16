/**
 * @file navigational_parser_types.hpp
 * Types for representing a program in the navigational language.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */

#ifndef NAVIGATIONAL_PARSER_TYPES_H
#define NAVIGATIONAL_PARSER_TYPES_H

#include <boost/spirit/include/qi.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <vector>
#include <string>

namespace csvp {
namespace navigational {
namespace parser {

namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

//
// Pattern structures
//
struct pattern;
struct pattern_alternation;
struct pattern_next;
struct pattern_any;
struct pattern_variable;
using pattern_expression = boost::variant<
  pattern_next,
  pattern_any,
  pattern_variable
>;

struct pattern {
  std::vector<pattern_expression> expressions;

  std::vector<std::string> var_names() const;
  bool has_backreference() const;
  bool operator==(const pattern& rhs) const;
};

struct pattern_alternation {
  std::vector<std::string> words;

  pattern_alternation() {}
  pattern_alternation(const std::vector<std::string>& words_) : words(words_) {}

  bool operator==(const pattern_alternation& rhs) const;
};

struct pattern_next {
  int count;
  pattern_alternation alternation;

  pattern_next() : count(1) {}
  pattern_next(const pattern_alternation& alternation_ )
    : count(1), alternation(alternation_) {}

  bool operator==(const pattern_next& rhs) const;
};

struct pattern_any {
  pattern_alternation alternation;

  pattern_any() {}
  pattern_any(const pattern_alternation& alternation_ ) : alternation(alternation_) {}

  bool operator==(const pattern_any& rhs) const;
};

struct pattern_variable {
  bool content;
  std::string variable;
  pattern_next next;

  pattern_variable() : content(false) {}
  pattern_variable(const std::string& variable_, const pattern_next& next_)
    : content(false), variable(variable_), next(next_) {}
  pattern_variable(const std::string& variable_, const pattern_alternation& alternation_)
    : content(false), variable(variable_), next(alternation_) {}

  bool operator==(const pattern_variable& rhs) const;
};

} // namespace parser
} // namespace navigational
} // namespace csvp

//
// Fusion adapters
//
BOOST_FUSION_ADAPT_STRUCT(
    csvp::navigational::parser::pattern,
    (std::vector<csvp::navigational::parser::pattern_expression>, expressions)
)

BOOST_FUSION_ADAPT_STRUCT(
    csvp::navigational::parser::pattern_alternation,
    (std::vector<std::string>, words)
)

BOOST_FUSION_ADAPT_STRUCT(
    csvp::navigational::parser::pattern_next,
    (int, count)
    (csvp::navigational::parser::pattern_alternation, alternation)
)

BOOST_FUSION_ADAPT_STRUCT(
    csvp::navigational::parser::pattern_any,
    (csvp::navigational::parser::pattern_alternation, alternation)
)

BOOST_FUSION_ADAPT_STRUCT(
    csvp::navigational::parser::pattern_variable,
    (bool, content)
    (std::string, variable)
    (csvp::navigational::parser::pattern_next, next)
)

#endif
