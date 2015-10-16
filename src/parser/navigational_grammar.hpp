/**
 * @file navigational_grammar.hpp
 * Grammar for the navigational language.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */

#ifndef NAVIGATIONAL_GRAMMAR_H
#define NAVIGATIONAL_GRAMMAR_H

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_alternative.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_container.hpp>

#include "navigational_parser_types.hpp"

//
// Parser grammar
//

namespace csvp {
namespace navigational {
namespace parser {

template <class Iterator>
struct navigational_grammar
: qi::grammar<Iterator, pattern(), ascii::blank_type> {
  navigational_grammar()
    : navigational_grammar::base_type(pattern_, "navigational_pattern") {
    using qi::lexeme;
    using qi::lit;
    using ascii::char_;
    using qi::int_;
    using ascii::alpha;
    using ascii::alnum;
    using ascii::digit;
    using ascii::no_case;
    using qi::uint_;
    using qi::hex;
    using qi::attr;
    using qi::eps;
    using qi::eol;
    using qi::omit;

    using namespace qi::labels;
    using phoenix::construct;
    using phoenix::val;
    using phoenix::insert;
    using phoenix::push_back;
    using phoenix::begin;
    using phoenix::end;
    using phoenix::at_c;

    pattern_ =
            '/'
         >  pattern_expression_ % '/'
         >  '/'
    ;

    pattern_expression_ =
            pattern_next_
         |  pattern_any_
         |  pattern_variable_
    ;

    pattern_next_ =
            lit("next")
         >  (('{' >> int_ >> '}' | attr(1))) // a count: 1 by default
         >  lit("(")
         >  pattern_alternation_
         >  lit(")")
    ;

    pattern_any_ =
            lit("any(")
         >  pattern_alternation_
         >  lit(")")
    ;

    pattern_variable_ =
           ('?' >> attr(false)
         |  '&' >> attr(true))
         >  identifier > ':'
         >  pattern_next_
    ;

    pattern_alternation_ =
            eps
         > (
              shortcut [
                insert(at_c<0>(_val), end(at_c<0>(_val)), begin(_1), end(_1))
              ]
           |  word [
                push_back(at_c<0>(_val), _1)
              ]
           )
         % '|'
    ;

    word = lexeme[
      +(unesc_char | (char_ - '|' - ')' - '\\' - '\n') | "\\x" >> hex)
    ];

    unesc_char.add("\\a", "\a")("\\b", "\b")("\\f", "\f")("\\n", "\n")
                  ("\\r", "\r")("\\t", "\t")("\\v", "\v")("\\\\", "\\")
                  ("\\|", "|")("\\(", "(")("\\)",")")("\\$","\\$")("\\^","\\^");

    identifier = lexeme[alpha > *(alnum)];

    pattern_.name("pattern");
    pattern_expression_.name("pattern_expression");
    pattern_next_.name("next_expression");
    pattern_any_.name("any_expression");
    pattern_variable_.name("variable_expression");
    pattern_alternation_.name("pattern_alternation");
    word.name("word");
    unesc_char.name("escaped_char");
    identifier.name("identifier");

    // Uncomment for debugging
    /* debug(pattern_); */
    /* debug(pattern_expression_); */
    /* debug(pattern_next_); */
    /* debug(pattern_any_); */
    /* debug(pattern_variable_); */
    /* debug(pattern_alternation_); */
    /* debug(word); */
    /* debug(identifier); */

    qi::on_error<qi::fail>(
      pattern_,
      std::cerr
        << val("Error: expecting ")
        << _4
        << val(" here: \"")
        << construct<std::string>(_3, _2)
        << val("\"")
        << std::endl
    );
  }

  qi::rule<Iterator, pattern(), ascii::blank_type> pattern_;
  qi::rule<Iterator, pattern_alternation()> pattern_alternation_;
  qi::rule<Iterator, pattern_next(), ascii::blank_type> pattern_next_;
  qi::rule<Iterator, pattern_any(), ascii::blank_type> pattern_any_;
  qi::rule<Iterator, pattern_variable(), ascii::blank_type> pattern_variable_;
  qi::rule<Iterator, pattern_expression(), ascii::blank_type> pattern_expression_;
  qi::rule<Iterator, std::string(), ascii::blank_type> identifier;
  qi::rule<Iterator, std::string()> word;
  qi::symbols<char const, char const*> unesc_char;
  qi::symbols<char const, std::vector<std::string>> shortcut;
};

} // namespace parser
} // namespace navigational
} // namespace csvp

#endif
