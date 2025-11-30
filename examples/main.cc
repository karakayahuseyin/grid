/**
 * @file main.cc
 * @brief Simple multithreaded HTTP server example
 * 
 * Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
 * Licensed under the MIT License. Part of the Revak project.
 */

#include "revak/Socket.h"
#include "revak/ThreadPool.h"
#include "revak/Response.h"

#include <iostream>
#include <string>
#include <memory>
#include <unistd.h> 

#define NUMS_THREAD 4

void HandleClient(std::shared_ptr<revak::Socket> client) {
  // std::string body = "Hello from Thread "
  //   + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id()));

  // std::string response =
  //       "HTTP/1.1 200 OK\r\n"
  //       "Content-Type: text/plain\r\n"
  //       "Content-Length: " + std::to_string(body.size()) + "\r\n"
  //       "Connection: close\r\n"
  //       "\r\n"
  //       + body;

	revak::Response resp;
	resp.SetStatus(200);
	resp.SetHeader("Content-Type", "text/plain");
	std::string body = "Hello from Thread "
		+ std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id()));
	resp.SetHeader("Content-Length", std::to_string(body.size()));
	resp.SetHeader("Connection", "close");
	resp.SetBody(body);

	::write(client->NativeHandle(), resp.ToString().c_str(), resp.ToString().size());
}    

int main() {
  try {
		std::cout << "[Main] Starting Server with " << NUMS_THREAD << " threads." << std::endl;
		revak::ThreadPool pool(NUMS_THREAD);

		revak::Socket server;
		server.Bind(8080);
		server.Listen();

		std::cout << "[Main] Server is listening on port 8080" << std::endl;

		while (true) {
			revak::Socket client = server.Accept();
			
			// Use shared_ptr to manage client socket lifetime in threads
			auto shared_client = std::make_shared<revak::Socket>(std::move(client));

			// Enqueue client handling task to the thread pool
			pool.Enqueue([shared_client] {
				HandleClient(shared_client);
			});
		}
	} catch (const std::exception& e) {
		std::cerr << "[Main] Server failed to start: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}