/**
 * @file Router.cc
 * @brief Router class implementation
 * 
 * Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
 * Licensed under the MIT License. Part of the Revak project.
 */

#include "revak/Router.h"
#include "revak/Logger.h"

namespace revak {

bool Router::AddRoute(const std::string& method, const std::string& path, Handler handler) {
  // Basic validation
  if (method.empty() || path.empty() || !handler) {
    Logger::Instance().Log(Logger::Level::ERROR, "Failed to add route: Invalid parameters");
    return false;
  }

  // Simple validation for path
  if (path[0] != '/') {
    Logger::Instance().Log(Logger::Level::ERROR, "Failed to add route: Path must start with '/'");
    return false;
  }

  // Check for existing route
  for (const auto& route : routes_) {
    if (route.method == method && route.path == path) {
      Logger::Instance().Log(Logger::Level::WARNING, "Route already exists: " + method + " " + path);
      return false;
    }
  }

  routes_.emplace_back(Route{method, path, std::move(handler)});
  Logger::Instance().Log(Logger::Level::INFO, "Route added: " + method + " " + path);
  return true;
}

Response Router::Dispatch(const Request& request) {
  Logger::Instance().Log(Logger::Level::INFO, "Request received: " + request.Method() + " " + request.Path());
  for (const auto& route : routes_) {
    if (route.method == request.Method() && route.path == request.Path()) {
      return route.handler(request);
    }
  }

  Response response;
  response.SetStatus(404);
  response.SetBody("404 Not Found\n");
  return response;
}

} // namespace revak