/**
 * @file cli.hpp
 * Command line interface methods.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#ifndef CLI_H
#define CLI_H

#include <string>

namespace csvp {
namespace navigational {

/**
 * Load file contents into string.
 */
std::string file_to_str(const char *filename);

} // namespace navigational
} // namespace csvp

#endif
