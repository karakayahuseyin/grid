/**
 * @file main.cc
 * @brief Simple multithreaded HTTP server example
 * 
 * Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
 * Licensed under the MIT License. Part of the Revak project.
 */

#include "revak/Server.h"
#include "revak/Logger.h"

int main() {
	revak::Server server(8080, 4);

	server.Get("/hello", [](const revak::Request& req) {
		revak::Response res;
		res.SetStatus(200);
		res.SetBody(req.Method() + " " + req.Path() + " says Hello, World!\n");
		res.SetHeader("Content-Type", "text/plain");
		res.SetHeader("Connection", "close");
		return res;
	});
	
	server.Run();
	return 0;
}