/**
 * @file navigational_parser.hpp
 * Parser for the navigational language.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */

#ifndef NAVIGATIONAL_PARSER_H
#define NAVIGATIONAL_PARSER_H

#include "navigational_parser_types.hpp"
#include <string>
#include <istream>

namespace csvp {
namespace navigational {
namespace parser {

class navigational_parser {
public:
  pattern parse(const std::string& input) const;
  pattern parse(std::istream& input) const;
};

}
}
}

#endif
