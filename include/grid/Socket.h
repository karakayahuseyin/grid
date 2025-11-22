/**
 * @file Socket.h
 * @brief Socket class declaration
 * 
 * Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
 * Licensed under the MIT License. Part of the Grid project.
 */

#pragma once

#include <cstdint>
#include <sys/socket.h>

namespace grid {

class Socket {
public:
  /// @brief Create new TCP socket (socket() syscall)
  Socket();

  /// @brief Close Socket if open (close() syscall)
  ~Socket();

  // Delete Copy
  Socket(const Socket&) = delete;
  Socket& operator=(const Socket&) = delete;

  // Move Semantics
  Socket(Socket&& other) noexcept;
  Socket& operator=(Socket&& other) noexcept;

  /// @brief Connect port and IP to Socket
  void Bind(uint16_t port);

  /// @brief Puts the Socket to listening mode
  void Listen();

  /// @brief Accepts incoming connection and returns new Socket object
  [[nodiscard]] Socket Accept();

  /// @brief Puts to Socket in non-blocking mode
  void SetNonBlocking();

  /// @brief Closes the socket
  void Close();

  /// @brief Access for raw file descriptor
  [[nodiscard]] int NativeHandle() const { return fd_; }

private:
  /// @brief Raw file descriptor
  int fd_{-1};

  /**
   * @brief Wraps an exist file descriptor
   * @param fd File descriptor
   */
  explicit Socket(int fd);
};

} // namespace grid