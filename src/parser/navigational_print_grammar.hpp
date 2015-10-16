/**
 * @file navigational_print_grammar.hpp
 * Grammar for printing the navigational language.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */

#ifndef NAVIGATIONAL_PRINT_GRAMMAR_H
#define NAVIGATIONAL_PRINT_GRAMMAR_H

#include "navigational_parser_types.hpp"
#include <boost/spirit/include/karma.hpp>

namespace csvp {
namespace navigational {
namespace parser {

namespace karma = boost::spirit::karma;

template <class Iterator>
struct navigational_print_grammar
: karma::grammar<Iterator, pattern()> {
  navigational_print_grammar() : navigational_print_grammar::base_type(pattern_) {
    using karma::print;
    using karma::hex;
    using karma::alpha;
    using karma::alnum;
    using karma::int_;
    using karma::bool_;
    using karma::lit;
    using karma::eps;
    using karma::omit;

    pattern_ =
            '/'
        <<  pattern_expression_ % '/'
        <<  '/'
    ;

    pattern_expression_ =
            pattern_next_
         |  pattern_any_
         |  pattern_variable_
    ;

    pattern_next_ =
            lit("next")
        <<  (omit[int_(1)] | '{' << int_ << '}')
        <<  lit("(")
        <<  pattern_alternation_
        <<  lit(")")
    ;

    pattern_any_ =
            lit("any(")
        <<  pattern_alternation_
        <<  lit(")")
    ;

    pattern_variable_ =
           (omit[bool_(false)] << '?'
         |  omit[bool_(true)] << '&')
        <<  identifier << ':'
        <<  pattern_next_
    ;

    pattern_alternation_ =
            eps << word % '|'
    ;

    word = +(unesc_char | print | "\\x" << hex);

    unesc_char.add('\a', "\\a")('\b', "\\b")('\f', "\\f")('\n', "\\n")
                  ('\r', "\\r")('\t', "\\t")('\v', "\\v")('\\', "\\\\")
                  ('|', "\\|")('(', "\\(")(')', "\\)");

    /* identifier = alpha << *(alnum); */
    identifier = karma::string;
  }

  karma::rule<Iterator, pattern()> pattern_;
  karma::rule<Iterator, pattern_alternation()> pattern_alternation_;
  karma::rule<Iterator, pattern_next()> pattern_next_;
  karma::rule<Iterator, pattern_any()> pattern_any_;
  karma::rule<Iterator, pattern_variable()> pattern_variable_;
  karma::rule<Iterator, pattern_expression()> pattern_expression_;
  karma::rule<Iterator, std::string()> identifier;
  karma::rule<Iterator, std::string()> word;
  karma::symbols<char, char const*> unesc_char;
};

}
}
}
#endif
