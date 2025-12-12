/**
 * @file Server.h
 * @brief Server class declaration
 * 
 * Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
 * Licensed under the MIT License. Part of the Revak project.
 */

#pragma once

#include "Router.h"
#include "Socket.h"
#include "ThreadPool.h"

#include <atomic>

namespace revak {

class Server {
public:
  /// @brief Create new Server instance
  /// @param port Port number to bind the server
  /// @param thread_nums Number of threads in the thread pool
  explicit Server(uint16_t port, size_t thread_nums = 4);

  /// @brief Destructor to stop the server
  ~Server();

  /// @brief  Starts the server
  void Run();

  /// @brief Stops the server
  bool Stop();

  /// @brief Add a new route to the server's router
  /// @param method HTTP method (e.g., GET, POST)
  /// @param path Request path
  /// @param handler Handler function for the route
  bool AddRoute(const std::string& method, const std::string& path, Handler handler);

  /// @brief Shortcut for adding GET route
  /// @param path Request path
  /// @param handler Handler function for the route
  bool Get(const std::string& path, Handler handler);

  /// @brief Shortcut for adding POST route
  /// @param path Request path
  /// @param handler Handler function for the route
  bool Post(const std::string& path, Handler handler);

  /// @brief Shortcut for adding PUT route
  /// @param path Request path
  /// @param handler Handler function for the route
  bool Put(const std::string& path, Handler handler);
  
  /// @brief Shortcut for adding DELETE route
  /// @param path Request path
  /// @param handler Handler function for the route
  bool Delete(const std::string& path, Handler handler);

private:
  uint16_t port_;
  size_t thread_nums_;
  
  std::atomic<bool> running_{false};
  
  Socket socket_;
  ThreadPool thread_pool_;
  Router router_;
};

} // namespace revak 