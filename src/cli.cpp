/**
 * @file cli.cpp
 * Command line interface methods.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include "cli.hpp"

#include <string>
#include <fstream>
#include <stdexcept>

using namespace std;

namespace csvp {
namespace navigational {

string file_to_str(const char *filename) {
  ifstream in(filename, ios::in | ios::binary);
  if (in) {
    string contents;
    in.seekg(0, ios::end);
    contents.resize(in.tellg());
    in.seekg(0, ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return contents;
  }
  throw runtime_error("Error loading file.");
}

} // namespace navigational
} // namespace csvp
