/**
 * @file main_datalog.cpp
 * Datalog evaluation program main file.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include "cli.hpp"
#include "logger.hpp"
#include "parser/datalog_parser.hpp"
#include "eval/program_eval.hpp"

using namespace std;
using namespace csvp::navigational;
namespace fs = boost::filesystem;

void usage(char *program_name) {
  cerr << "usage: " << program_name << " DOCUMENT_PATH PROGRAM_PATH" << endl
    << endl;
  cerr << "  -b,\tprint benchmark times (omits the query result printing)"
    << endl;
}

int main(int argc, char *argv[]) {
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

  if (argc < 2 || argc > 3) {
    usage(program_name);
    return 1;
  }

  logger l(benchmark);

  // Read program into memory
  string program_str;
  const char *program_path = argc == 2 ? argv[1] : argv[2];
  try {
    program_str = file_to_str(program_path);
  } catch (exception& e) {
    cerr << "An error occurred while opening file \"" << program_path << "\"."
      << endl;
    return 1;
  }

  // Parse program
  parser::program program;
  try {
    program = parser::datalog_parser().parse(program_str);
  } catch (exception& e) {
    cerr << "An error occurred parsing the program." << endl;
    return 1;
  }

  l.log("Program parsed");

  // Read document into memory
  fs::path document_bpath;
  const char *document_path;
  // Document as an argument
  if (argc == 3) {
    document_path = argv[1];
  } else {
    // Document in the program
    if (!program.document) {
      cerr <<
        "Document not specified, please specify as argument or in program"
        << endl;
      return 1;
    }
    auto program_dir = fs::path(program_path).parent_path();
    document_bpath =
      fs::absolute(program.document->c_str(), program_dir.c_str());
    document_path = document_bpath.c_str();
  }

  string document;
  try {
    document = file_to_str(document_path);
  } catch (exception& e) {
    cerr << "An error occurred while opening file \"" << document_path << "\"."
      << endl;
    return 1;
  }

  l.log("Document loaded");

  // Evaluate program
  eval::program_eval evaluator(document);
  auto predicate_spans = evaluator.eval(program);

  l.log("Pattern evaluation total");

  // Print predicates
  if (!benchmark) {
    cout << "- - -\n";
    for (auto& pair : predicate_spans) {
      cout << pair.first;

      for (auto& span : pair.second) {
        cout << '\t' << span << '\t';
        cout.write(document.data() + span.start, span.length());
        cout << '\n';
      }

      if (pair.second.empty()) {
        cout << '\n';
      }

      cout << "- - -\n";
    }
  }

  return 0;
}
