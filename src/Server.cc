/**
 * @file Server.cc
 * @brief Server class implementation
 * 
 * Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
 * Licensed under the MIT License. Part of the Revak project.
 */

#include "revak/Server.h"
#include "revak/Logger.h"

#include <unistd.h>

namespace revak {

Server::Server(uint16_t port, size_t thread_nums)
  : port_(port), thread_nums_(thread_nums), running_(false), thread_pool_(thread_nums)  {
  socket_ = Socket();
  if (!socket_.Bind(port_)) {
    Logger::Instance().Log(Logger::Level::ERROR, "Failed to bind server to port " + std::to_string(port_));
    return;
  }
  if (!socket_.Listen()) {
    Logger::Instance().Log(Logger::Level::ERROR, "Failed to listen on port " + std::to_string(port_));
    return;
  }
  Logger::Instance().Log(Logger::Level::INFO, "Server started on port " + std::to_string(port_));
}

Server::~Server() {
  Logger::Instance().Log(Logger::Level::INFO, "Server stopping...");
  Stop();
}

void Server::Run() {
  running_ = true;
  while (running_) {
    // Accept incoming connection
		Socket client = socket_.Accept();
    if (client.NativeHandle() < 0) {
      continue; // Accept failed, try next
    }
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
        Response res = router_.Dispatch(req);
        ::write(shared_client->NativeHandle(), res.ToString().c_str(), res.ToString().size());
        Logger::Instance().Log(Logger::Level::INFO, "Handled " + req.Method() + " " + req.Path() + " with status " + std::to_string(res.GetStatusCode()));
      }
		});
	}
}

bool Server::AddRoute(const std::string& method, const std::string& path, Handler handler) {
  if (running_) {
    Logger::Instance().Log(Logger::Level::WARNING, "Cannot add route while server is running.");
    return false;
  }
  
  return router_.AddRoute(method, path, handler);
}

bool Server::Get(const std::string& path, Handler handler) {
  return router_.AddRoute("GET", path, handler);
}

bool Server::Post(const std::string& path, Handler handler) {
  return router_.AddRoute("POST", path, handler);
}

bool Server::Put(const std::string& path, Handler handler) {
  return router_.AddRoute("PUT", path, handler);
}

bool Server::Delete(const std::string& path, Handler handler) {
  return router_.AddRoute("DELETE", path, handler);
}

bool Server::Stop() {
  running_ = false;
  socket_.Close();
  Logger::Instance().Log(Logger::Level::INFO, "Server stopped.");
  return true;
}

} // namespace revak