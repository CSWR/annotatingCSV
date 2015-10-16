/**
 * @file logger.cpp
 * Logs and outputs time measurements.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */

#include "logger.hpp"
#include <iostream>

namespace chrono = std::chrono;
using chrono::steady_clock;
using chrono::duration;
using std::string;
using std::cout;

namespace csvp {
namespace navigational {

logger::logger(bool active)
: active_(active), last_time_(steady_clock::now()) {}

void logger::log() {
  if (!active_) return;
  last_time_ = steady_clock::now();
}

void logger::log(const string& message) {
  if (!active_) return;
  auto current_time = steady_clock::now();
  auto time_diff = current_time - last_time_;

  cout << message << ": "
       << duration<double, std::milli>(time_diff).count()
       << " ms" << '\n';

  last_time_ = current_time;
}

} // namespace navigational
} // namespace csvp
