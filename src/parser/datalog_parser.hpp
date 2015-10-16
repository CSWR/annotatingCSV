/**
 * @file datalog_parser.hpp
 * Parser for the datalog language.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */

#ifndef DATALOG_PARSER_H
#define DATALOG_PARSER_H

#include "datalog_parser_types.hpp"
#include <string>
#include <istream>

namespace csvp {
namespace navigational {
namespace parser {

class datalog_parser {
public:
  program parse(const std::string& input) const;
  program parse(std::istream& input) const;
};

}
}
}

#endif
