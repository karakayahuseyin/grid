/**
 * @file Logger.h
 * @brief Logger class declaration
 * 
 * Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
 * Licensed under the MIT License. Part of the Revak project.
 */

#pragma once

#include <string>
#include <format>
#include <mutex>
#include <queue>
#include <thread>
#include <atomic>
#include <condition_variable>

namespace revak {

class Logger {
public:
  /// @brief Get the singleton instance of Logger
  static Logger& Instance();
  
  // Disable copy and assignment
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  /// @brief Log levels
  enum class Level {
    INFO,
    WARNING,
    ERROR
  };

  /// @brief Log a message with the specified level
  /// @param level Log level (INFO, WARNING, ERROR)
  /// @param message Message to log
  void Log(Level level, const std::string& message);

private:
  /// @brief Private constructor for singleton pattern
  Logger();
  ~Logger();

  // Mutex for thread-safe logging
  std::mutex queue_mutex_;

  // Thread for asynchronous logging
  std::thread log_thread_;

  // Queue for log messages
  std::queue<std::string> log_queue_;

  // Atomic variable for stopping logging
  std::atomic<bool> stop_logging_{false};

  // Condition variable for log notification
  std::condition_variable log_condition_;
};

} // namespace revak