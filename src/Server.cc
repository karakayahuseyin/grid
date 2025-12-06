/**
 * @file Server.cc
 * @brief Server class implementation
 * 
 * Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
 * Licensed under the MIT License. Part of the Revak project.
 */

#include "revak/Server.h"

#include <stdexcept>

namespace revak {

Server::Server(uint16_t port, size_t thread_nums)
  : port_(port), thread_nums_(thread_nums),running_(false), thread_pool_(thread_nums)  {
  socket_ = Socket();
  socket_.Bind(port_);
  socket_.Listen();
}

Server::~Server() {
  Stop();
}

void Server::Run() {
  running_ = true;
  while (running_) {
    // Accept incoming connection
		Socket client = socket_.Accept();
		
    // Use shared_ptr to manage client socket lifetime in threads
		auto shared_client = std::make_shared<Socket>(std::move(client));

		// Enqueue client handling task to the thread pool
		thread_pool_.Enqueue([shared_client, this] {
			// Buffer to store incoming data
      char buffer[4096];
      // Tech debt: Buffer overflow handling needed
      ssize_t bytes_read = ::read(shared_client->NativeHandle(), buffer, sizeof(buffer));
      if (bytes_read < 0) {
        perror("read");
        return;
      } else if (bytes_read == 0) {
        // Connection closed by client
        return;
      } else {
        std::string request_data(buffer, bytes_read);
        Request req = Request(request_data);
        Response res = this->router_.Dispatch(req);
        ::write(shared_client->NativeHandle(), res.ToString().c_str(), res.ToString().size());
      }
		});
	}
}

void Server::AddRoute(const std::string& method, const std::string& path, Handler handler) {
  if (running_) {
    throw std::runtime_error("Cannot add routes while server is running");
  }
  router_.AddRoute(method, path, handler);
}

void Server::Get(const std::string& path, Handler handler) {
  AddRoute("GET", path, handler);
}

void Server::Post(const std::string& path, Handler handler) {
  AddRoute("POST", path, handler);
}

void Server::Put(const std::string& path, Handler handler) {
  AddRoute("PUT", path, handler);
}

void Server::Delete(const std::string& path, Handler handler) {
  AddRoute("DELETE", path, handler);
}

void Server::Stop() {
  running_ = false;
  socket_.Close();
}

} // namespace revak