/**
 * @file main.cc
 * @brief Simple multithreaded HTTP server example
 * 
 * Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
 * Licensed under the MIT License. Part of the Revak project.
 */

#include "revak/Server.h"

#include <iostream>
#include <stdexcept>

int main() {
	try {
		revak::Server server(8080, 4);

		// Define a simple GET route
		server.Get("/hello", [](const revak::Request& req) {
			revak::Response res;
			res.SetStatus(200);
			res.SetBody(req.Method() + " " + req.Path() + " says Hello, World!\n");
			res.SetHeader("Content-Type", "text/plain");
			res.SetHeader("Connection", "close");
			return res;
		});
		// Start the server
		std::cout << "Starting server on port 8080..." << std::endl;
		server.Run();
	} catch (const std::exception& e) {
		std::runtime_error("Server error: " + std::string(e.what()));
		return 1;
	}
	return 0;
}