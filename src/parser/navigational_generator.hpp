/**
 * @file navigational_generator.hpp
 * Generator for printing the navigational language.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */

#ifndef NAVIGATIONAL_GENERATOR_H
#define NAVIGATIONAL_GENERATOR_H

#include "navigational_parser_types.hpp"
#include <string>
#include <ostream>

namespace csvp {
namespace navigational {
namespace parser {

class navigational_generator {
public:
  std::string generate(const pattern& pattern) const;
};

}
}
}

#endif
