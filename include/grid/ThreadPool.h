/**
 * @file ThreadPool.h
 * @brief Server's thread pool for handling concurrent tasks.
 * 
 * Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
 * Licensed under the MIT License. Part of the Grid project.
 */

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <stdexcept>

namespace grid {

class ThreadPool {
public:
  /**
   * @brief Create thread pool with given number of threads
   * @param numThreads Number of threads in the pool
   */
  explicit ThreadPool(size_t numThreads);

  /// @brief Destructor to join all threads
  ~ThreadPool();

  /// @brief Enqueue a new task to the thread pool
  void Enqueue(std::function<void()> task);

private:
  // Worker threads
  std::vector<std::thread> workers_;

  // Mutex for task queue
  std::mutex queue_mutex_;

  // Flag to stop the pool
  bool stop_;

  // Task queue
  std::queue<std::function<void()>> tasks_;

  // Condition variable for task notification
  std::condition_variable condition_;
};

} // namespace grid