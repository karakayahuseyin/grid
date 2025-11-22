/**
 * @file Socket.cc
 * @brief Socket class implementation
 * 
 * Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
 * Licensed under the MIT License. Part of the Grid project.
 */

#include "grid/Socket.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <cstring>

namespace grid {

Socket::Socket() {
	// Create TCP Socket (SOCK_STREAM) with IPv4 protocols (AF_INET) 
	fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if (fd_ < 0) {
		throw std::runtime_error("Failed to create socket");
	}
	
	int opt = 1;
	/**
	 * With SO_REUSEADDR, the same port can be reused 
	 * when the server is restarted immediately after shutdown.
	 */
	if (::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		Close();
		throw std::runtime_error("Failed to set SO_REUSEADDR");
	}
}

Socket::Socket(int fd) : fd_(fd) {
	if (fd_ < 0) {
		std::runtime_error("Invalid file descriptor passed to Socket");
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

void Socket::Bind(uint16_t port) {
	struct sockaddr_in addr{};
	std::memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	if (::bind(fd_, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		throw std::runtime_error("Failed to bind socket");
	}
}

void Socket::Listen() {
  // SOMAXCONN: Maximum wait queue length allowed by the operating system.
  // Usually it is 128 or 4096.
  if (::listen(fd_, SOMAXCONN) < 0) {
      throw std::runtime_error("Failed to listen on socket");
  }
}

Socket Socket::Accept() {
	struct sockaddr_in client_addr{};
	socklen_t client_len = sizeof(client_addr);

	// Accept client connection
	// This call (by default) blocks until a connection request
	int client_fd = ::accept(fd_, (struct sockaddr*)&client_addr, &client_len);

	if (client_fd < 0) {
		throw std::runtime_error("Failed to accept connection");
	}

	// Wrap and return the new fd (Move Semantics implementation)
	return Socket(client_fd);
}

void Socket::SetNonBlocking() {
	// Get exist flags
	int flags = ::fcntl(fd_, F_GETFL, 0);

	if (flags == -1) {
		throw std::runtime_error("Failed to get socket flags");
	}

	// Add O_NONBLOCK flag
	if (::fcntl(fd_, F_SETFL, flags | O_NONBLOCK) == -1) {
    throw std::runtime_error("Failed to set non-blocking mode");
  }
}

void Socket::Close() {
	if (fd_ != -1) {
		::close(fd_); // syscall
		fd_ = -1;
	}
}

} // namespace grid