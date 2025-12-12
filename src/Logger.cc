/**
 * @file Logger.cc
 * @brief Implementation of the Logger class for asynchronous logging.
 * 
 * Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
 * Licensed under the MIT License. Part of the Revak project.
 */

#include "revak/Logger.h"

#include <chrono>
#include <iostream>
#include <format>

namespace revak {

Logger& Logger::Instance() {
  static Logger instance;
  return instance;
}

Logger::Logger() {
  log_thread_ = std::thread([this]() {
    while(true) {
      {
        // Lock the queue mutex
        std::unique_lock<std::mutex> lock(this->queue_mutex_);

        // Wait until there is a log message or stop signal
        this->log_condition_.wait(lock, [this] {
          return this->stop_logging_.load() || !this->log_queue_.empty();
        });

        // If stopping and no messages left, exit the loop
        if (this->stop_logging_.load() && this->log_queue_.empty()) {
          break;
        }

        std::string message = std::move(this->log_queue_.front());
        this->log_queue_.pop();

        std::cout << message << std::endl;
      } // Scope for lock ends here
    }
  });
}

Logger::~Logger() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    stop_logging_ = true;
  }
  log_condition_.notify_all();

  if (log_thread_.joinable()) {
    log_thread_.join();
  }
}

void Logger::Log(Level level, const std::string& message) {
  // [Timestamp] [Level] [Message]
  auto now = std::chrono::system_clock::now();
  auto now_ms = std::chrono::floor<std::chrono::milliseconds>(now);
  std::string timestamp = std::format("{:%Y-%m-%d %H:%M:%S}", now_ms);

  std::string level_str;
  switch (level) {
    case Level::INFO: level_str = "INFO"; break;
    case Level::WARNING: level_str = "WARN"; break;
    case Level::ERROR: level_str = "ERR "; break;
  }

  std::string log_entry = std::format("[{}|{}] {}", timestamp, level_str, message);

  {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    log_queue_.push(log_entry);
    log_condition_.notify_one();
  }
}

} // namespace revak