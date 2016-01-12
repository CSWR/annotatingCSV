/**
 * @file datalog_grammar.hpp
 * Grammar for the modified datalog.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */

#ifndef DATALOG_GRAMMAR_H
#define DATALOG_GRAMMAR_H

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_alternative.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>

#include "datalog_parser_types.hpp"
#include "navigational_grammar.hpp"

//
// Parser grammar
//

namespace csvp {
namespace navigational {
namespace parser {

template <class Iterator>
struct datalog_grammar
: qi::grammar<Iterator, program(), ascii::blank_type> {
  datalog_grammar() : datalog_grammar::base_type(program_, "datalog_program") {
    using qi::lexeme;
    using qi::lit;
    using ascii::char_;
    using ascii::alpha;
    using ascii::alnum;
    using ascii::no_case;
    using qi::uint_;
    using qi::hex;
    using qi::eps;
    using qi::eol;
    using qi::omit;
    using qi::matches;

    using namespace qi::labels;
    using phoenix::bind;
    using phoenix::construct;
    using phoenix::val;

    program_ =
            *(comment >> eol)
         >  preamble_
         >  *(comment >> eol)
         >  rule_ % +(comment >> eol)
         >  *(comment >> eol)
    ;

    preamble_ =
            -(document_statement_)
         >  *(regex_statement_ | anchor_statement_)
    ;

    document_statement_ =
            lit("DOCUMENT")
         >  '"' > q_string > '"'
         >  +(comment >> eol)
    ;

    regex_statement_ =
           (lit("REGEX")
         >  identifier
         >  '='
         >  '"' > q_string > '"'
         >  +(comment >> eol)
         )  [ bind(regex_shortcut.add, _1, _2) ]
    ;

    anchor_statement_ =
           (lit("ANCHOR")
         >  identifier
         >  '='
         >  lexeme[pattern_.word % '|']
         >  +(comment >> eol)
         )  [ bind(pattern_.shortcut.add, _1, _2) ]
    ;

    rule_ =
            predicate_
         >  lit(":-")
         >  clause_ % ','
         >  ';'
    ;

    clause_ =
            predicate_
         |  pattern_
         |  span_equality_
         |  content_equality_
         |  filter_
    ;

    predicate_ =
            identifier
        >>  lit("(?")
         >  identifier
         >  *(lit(",?") > identifier)
         >  ')'
    ;

    span_equality_ =
            matches[lit("not")]
        >>  '?' >> identifier
        >>  '='
        >>  '?' >> identifier
    ;

    content_equality_ =
            matches[lit("not")]
        >>  '&' >> identifier
        >>  '='
        >>  '&' >> identifier
    ;

    filter_ =
            matches[lit("not")]
        >>  '?' >> identifier
        >>  '.'
        >>  (regex_shortcut
         |  '"' >> q_string >> '"')
    ;

    identifier = lexeme[alpha > *(alnum)];

    comment = -omit['#' > *(char_ - eol)];

    q_string = +(unesc_char | (char_ - '"') | "\\x" >> hex);

    unesc_char.add("\\a", '\a')("\\b", '\b')("\\f", '\f')("\\n", '\n')
                  ("\\r", '\r')("\\t", '\t')("\\v", '\v')("\\\\", '\\')
                  ("\\\"", '"');

    program_.name("program");
    preamble_.name("preamble");
    document_statement_.name("document_statement");
    regex_statement_.name("regex_statement_");
    rule_.name("rule");
    clause_.name("clause");
    predicate_.name("predicate");
    span_equality_.name("span_equality");
    content_equality_.name("content_equality");
    filter_.name("filter");
    identifier.name("identifier");
    comment.name("comment");

    qi::on_error<qi::fail>(
      program_,
      std::cerr
        << val("Error: expecting ")
        << _4
        << val(" here: \"")
        << construct<std::string>(_3, _2)
        << val("\"")
        << std::endl
    );

    // Uncomment for debugging
    /* debug(program_); */
    /* debug(preamble_); */
    /* debug(document_statement_); */
    /* debug(regex_statement_); */
    /* debug(anchor_statement_); */
    /* debug(rule_); */
    /* debug(clause_); */
    /* debug(predicate_); */
    /* debug(span_equality_); */
    /* debug(content_equality_); */
    /* debug(filter_); */
    /* debug(identifier); */
    /* debug(comment); */
    /* debug(q_string); */
  }

  qi::rule<Iterator, program(), ascii::blank_type> program_;
  qi::rule<Iterator, boost::optional<std::string>(), ascii::blank_type>
    preamble_;
  qi::rule<Iterator, std::string(), ascii::blank_type> document_statement_;
  qi::rule<Iterator, void(), ascii::blank_type> regex_statement_;
  qi::rule<Iterator, void(), ascii::blank_type> anchor_statement_;
  qi::rule<Iterator, rule(), ascii::blank_type> rule_;
  qi::rule<Iterator, predicate(), ascii::blank_type> predicate_;
  qi::rule<Iterator, span_equality(), ascii::blank_type> span_equality_;
  qi::rule<Iterator, content_equality(), ascii::blank_type> content_equality_;
  qi::rule<Iterator, filter(), ascii::blank_type> filter_;
  qi::rule<Iterator, clause(), ascii::blank_type> clause_;
  navigational_grammar<Iterator> pattern_;
  qi::rule<Iterator, std::string(), ascii::blank_type> identifier;
  qi::rule<Iterator, void(), ascii::blank_type> comment;
  qi::rule<Iterator, std::string()> q_string;
  qi::symbols<char const, char const> unesc_char;
  qi::symbols<char const, std::string> regex_shortcut;
};

} // namespace parser
} // namespace navigational
} // namespace csvp

#endif
