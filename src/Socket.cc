/**
 * @file Socket.cc
 * @brief Socket class implementation
 * 
 * Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
 * Licensed under the MIT License. Part of the Revak project.
 */

#include "revak/Socket.h"
#include "revak/Logger.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

namespace revak {

Socket::Socket() {
	// Create TCP Socket (SOCK_STREAM) with IPv4 protocols (AF_INET) 
	fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if (fd_ < 0) {
		Logger::Instance().Log(Logger::Level::ERROR, "Failed to create socket");
		return;
	}
	
	int opt = 1;
	/**
	 * With SO_REUSEADDR, the same port can be reused 
	 * when the server is restarted immediately after shutdown.
	 */
	if (::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		Close();
		Logger::Instance().Log(Logger::Level::ERROR, "Failed to set SO_REUSEADDR option");
	}
}

Socket::Socket(int fd) : fd_(fd) {
	if (fd_ < 0) {
		Logger::Instance().Log(Logger::Level::ERROR, "Invalid file descriptor provided to Socket wrapper");
	}
}

Socket::~Socket() {
	Close();
}

Socket::Socket(Socket&& other) noexcept : fd_(other.fd_) {
  other.fd_ = -1;
}

Socket& Socket::operator=(Socket&& other) noexcept {
	if (this != &other) {
		Close(); // First clean the self resources
		fd_ = other.fd_; // Get new resources
		other.fd_ = -1; // Clean the old
	}
	return *this;
}

bool Socket::Bind(uint16_t port) {
	struct sockaddr_in addr{};
	std::memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	if (::bind(fd_, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		Logger::Instance().Log(Logger::Level::ERROR, "Failed to bind server to port " + std::to_string(port));
		return false;
	}
	return true;
}

bool Socket::Listen() {
  // SOMAXCONN: Maximum wait queue length allowed by the operating system.
  // Usually it is 128 or 4096.
  if (::listen(fd_, SOMAXCONN) < 0) {
      Logger::Instance().Log(Logger::Level::ERROR, "Failed to listen on socket");
      return false;
  }
	return true;
}

Socket Socket::Accept() {
	struct sockaddr_in client_addr{};
	socklen_t client_len = sizeof(client_addr);

	// Accept client connection
	// This call (by default) blocks until a connection request
	int client_fd = ::accept(fd_, (struct sockaddr*)&client_addr, &client_len);

	if (client_fd < 0) {
		Logger::Instance().Log(Logger::Level::ERROR, "Failed to accept incoming connection");
		return Socket(-1); // Return invalid Socket
	}

	// Wrap and return the new fd (Move Semantics implementation)
	return Socket(client_fd);
}

bool Socket::SetNonBlocking() {
	// Get exist flags
  int flags = ::fcntl(fd_, F_GETFL, 0);
  if (flags == -1) {
    Logger::Instance().Log(Logger::Level::ERROR, "Failed to get socket flags: " + std::string(std::strerror(errno)));
    return false;
  }

  if (::fcntl(fd_, F_SETFL, flags | O_NONBLOCK) == -1) {
    Logger::Instance().Log(Logger::Level::ERROR, "Failed to set non-blocking mode: " + std::string(std::strerror(errno)));
    return false;
  }

  return true;
}

bool Socket::Close() {
	if (fd_ != -1) {
		::shutdown(fd_, SHUT_RDWR); // syscall
		::close(fd_); // syscall
		fd_ = -1;
		return true;
	}
	return false;
}

} // namespace revak