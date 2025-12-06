/**
 * @file Router.cc
 * @brief Router class implementation
 * 
 * Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
 * Licensed under the MIT License. Part of the Revak project.
 */

#include "revak/Router.h"

namespace revak {

void Router::AddRoute(const std::string& method, const std::string& path, Handler handler) {
  routes_.emplace_back(Route{method, path, std::move(handler)});
}

Response Router::Dispatch(Request& request) {
  for (const auto& route : routes_) {
    if (route.method == request.Method() && route.path == request.Path()) {
      return route.handler(request);
    }
  }

  Response response;
  response.SetStatus(404);
  response.SetBody("404 Not Found");
  return response;
}

} // namespace revak