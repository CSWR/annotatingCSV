/**
 * @file main_navigational.cpp
 * Navigation language evaluation program main file.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include "cli.hpp"
#include "logger.hpp"
#include "parser/navigational_parser.hpp"
#include "eval/navigational_formula_eval.hpp"

using namespace std;
using namespace csvp::navigational;

void usage(char *program_name) {
  cerr << "usage: " << program_name << " DOCUMENT_PATH"
       << endl
       << endl;
  cerr << "  -b,\tprint benchmark times (omits the query result printing)"
       << endl;
}

int main(int argc, char **argv) {
  char *program_name = argv[0];

  // Usage message
  if (argc < 2) {
    usage(program_name);
    return 1;
  }

  bool benchmark = false;
  // Benchmark
  if (!strncmp(argv[1], "-b", 2)) {
    benchmark = true;
    argc--;
    argv++;
  }

  // Usage message
  if (argc != 2) {
    usage(program_name);
    return 1;
  }

  logger l(benchmark);

  // Read document into memory
  string document;
  try {
    document = file_to_str(argv[1]);
  } catch (exception& e) {
    cerr << "An error occurred while opening file \"" << argv[1] << "\"."
         << endl;
    return 1;
  }

  l.log("Document loaded");

  // Read pattern
  string input_pattern;
  getline(cin, input_pattern);
  parser::pattern pattern;

  // Parse pattern
  try {
    pattern = parser::navigational_parser().parse(input_pattern);
  } catch (exception& e) {
    cerr << "An error occurred parsing the pattern." << endl;
    return 1;
  }

  l.log("Pattern parsed");

  vector<pair<string, eval::span>> prefix;
  auto assignments = eval::navigational_formula_eval(pattern, document, prefix,
      benchmark);

  l.log("Pattern evaluation total");

  if (!benchmark) {
    cout << "- - -\n";

    // Print prefix if no groups.
    if (assignments.size() == 0) {
      // Print prefix
      for (auto& val : prefix) {
        cout << '?' << val.first << '\t' << val.second << '\t';
        cout.write(document.data() + val.second.start, val.second.length());
        cout << '\n';
      }
    }

    // Print answer
    vector<pair<string, eval::span>*> assignment;
    while (assignments.next(assignment)) {
      // Print prefix
      for (auto& val : prefix) {
        cout << '?' << val.first << '\t' << val.second << '\t';
        cout.write(document.data() + val.second.start, val.second.length());
        cout << '\n';
      }

      // Print the rest
      for (auto val : assignment) {
        cout << '?' << val->first << '\t' << val->second << '\t';
        cout.write(document.data() + val->second.start,
            val->second.end - val->second.start);
        cout << '\n';
      }

      cout << "- - -\n";
    }
  }

  return 0;
}
