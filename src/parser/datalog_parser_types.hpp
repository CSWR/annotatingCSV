/**
 * @file datalog_parser_types.hpp
 * Types for representing a program in modified datalog.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */

#ifndef DATALOG_PARSER_TYPES
#define DATALOG_PARSER_TYPES

#include <boost/spirit/include/qi.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/optional.hpp>
#include <vector>
#include <string>

#include "navigational_parser_types.hpp"

namespace csvp {
namespace navigational {
namespace parser {

namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

//
// Program structures
//
struct program;
struct rule;
struct predicate;
struct span_equality;
struct content_equality;
struct filter;
using clause = boost::variant<
  predicate,
  pattern,
  span_equality,
  content_equality,
  filter
>;

struct program {
  boost::optional<std::string> document;
  std::vector<rule> rules;

  bool operator==(const program& rhs) const;
};

struct predicate {
  std::string name;
  std::string variable;

  bool operator==(const predicate& rhs) const;
};

struct span_equality {
  bool negated;
  std::string first_var;
  std::string second_var;

  bool operator==(const span_equality& rhs) const;
};

struct content_equality {
  bool negated;
  std::string first_var;
  std::string second_var;

  bool operator==(const content_equality& rhs) const;
};


struct filter {
  bool negated;
  std::string variable;
  std::string regex;

  bool operator==(const filter& rhs) const;
};

struct rule {
  predicate pred;
  std::vector<clause> clauses;

  bool operator==(const rule& rhs) const;
};

} // namespace parser
} // namespace navigational
} // namespace csvp

//
// Fusion adapters
//
BOOST_FUSION_ADAPT_STRUCT(
    csvp::navigational::parser::program,
    (boost::optional<std::string>, document)
    (std::vector<csvp::navigational::parser::rule>, rules)
)

BOOST_FUSION_ADAPT_STRUCT(
    csvp::navigational::parser::rule,
    (csvp::navigational::parser::predicate, pred)
    (std::vector<csvp::navigational::parser::clause>, clauses)
)

BOOST_FUSION_ADAPT_STRUCT(
    csvp::navigational::parser::predicate,
    (std::string, name)
    (std::string, variable)
)

BOOST_FUSION_ADAPT_STRUCT(
    csvp::navigational::parser::span_equality,
    (bool, negated)
    (std::string, first_var)
    (std::string, second_var)
)

BOOST_FUSION_ADAPT_STRUCT(
    csvp::navigational::parser::content_equality,
    (bool, negated)
    (std::string, first_var)
    (std::string, second_var)
)

BOOST_FUSION_ADAPT_STRUCT(
    csvp::navigational::parser::filter,
    (bool, negated)
    (std::string, variable)
    (std::string, regex)
)

#endif
