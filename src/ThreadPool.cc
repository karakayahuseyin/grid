/**
 * @file ThreadPool.cc
 * @brief ThreadPool class implementation
 * 
 * Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
 * Licensed under the MIT License. Part of the Revak project.
 */

#include <revak/ThreadPool.h>

#include <thread>
#include <mutex>
#include <condition_variable>

namespace revak {

ThreadPool::ThreadPool(size_t numThreads) : stop_(false) {
  for (size_t i = 0; i < numThreads; i++) {
    workers_.emplace_back([this] {
      while(true) {
      std::function<void()> task;
      {
        // Lock the queue mutex
        std::unique_lock<std::mutex> lock(this->queue_mutex_);

        // Wait until there is a task or the pool is stopped. Don't use CPU
        this->condition_.wait(lock, [this] {
          return this->stop_ || !this->tasks_.empty();
        });

        // If stopping and no tasks left, exit the loop
        if (this->stop_ && this->tasks_.empty()) {
          return;
        }

        // Get the next task from the queue
        task = std::move(this->tasks_.front());
        this->tasks_.pop();
      } // Scope for lock ends here

      task();
      }
    });
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    stop_ = true;
  }
  condition_.notify_all();

  for (std::thread &w : workers_) {
    if (w.joinable()) {
      w.join();
    }
  }
}

void ThreadPool::Enqueue(std::function<void()> task) {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    tasks_.push(std::move(task));
  }
  condition_.notify_one();
}

} // namespace revak