/**
 * @file logger.hpp
 * Logs and outputs time measurements.
 *
 * @copyright Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
 * Distributed under the MIT License (see LICENSE.txt).
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <chrono>

namespace csvp {
namespace navigational {

/**
 * Log and output time measurements.
 */
class logger {
public:
  /**
   * Create new logger and register current time.
   * @param active Wether to emit output.
   */
  explicit logger(bool active);
  /**
   * Register current time.
   */
  void log();
  /**
   * Register current time and output time elapsed since last log.
   * @param message Message to output.
   */
  void log(const std::string& message);

private:
  bool active_; /**< Wether to emit output. */
  /** Last registered time */
  std::chrono::time_point<std::chrono::steady_clock> last_time_;
};

} // namespace navigational
} // namespace csvp

 #endif
