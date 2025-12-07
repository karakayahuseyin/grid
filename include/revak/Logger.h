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
  Logger();
  ~Logger();

  enum class Level {
    INFO,
    WARNING,
    ERROR
  };

  void Log(Level level, const std::string& message);

private:
  // Mutex for thread-safe logging
  std::mutex queue_mutex_;

  // Thread for asynchronous logging
  std::thread log_thread_;

  // Queue for log messages
  std::queue<std::string> log_queue_;

  // Atomic variable for stoping logging
  std::atomic<bool> stop_logging_{false};

  // Condition variable for log notification
  std::condition_variable log_condition_;
};

} // namespace revak