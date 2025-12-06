/**
 * @file Request.cc
 * @brief Request class implementation
 * 
 * Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
 * Licensed under the MIT License. Part of the Revak project.
 */

#include "revak/Request.h"

#include <string_view>
#include <algorithm>

namespace revak {

Request::Request(const std::string_view& request) {
  // Parse HTTP 1.1 request
  auto header_end = request.find("\r\n\r\n");
  
  // Header part
  std::string_view header_part = request.substr(0, header_end);
  body_ = std::string(request.substr(header_end + 4));

  size_t line_start = 0;
  size_t line_end = header_part.find("\r\n");

  // Request Line Parse (GET /index.html HTTP/1.1)
  if (line_end != std::string_view::npos) {
    std::string_view first_line = header_part.substr(0, line_end);

    // Method
    size_t method_end = first_line.find(' ');
    method_ = first_line.substr(0, method_end);

    // Path
    size_t path_end = first_line.find(' ', method_end + 1);
    path_ = first_line.substr(method_end + 1, path_end - (method_end + 1));
  }

  // Parse Headers
  while((line_end = header_part.find("\r\n", line_start)) != std::string_view::npos) {
    std::string_view line = header_part.substr(line_start, line_end - line_start);
    
    auto colon_pos = line.find(":");
    if (colon_pos != std::string_view::npos) {
      std::string_view key = line.substr(0, colon_pos);
      std::string_view val = line.substr(colon_pos + 1);

      if (!val.empty() && val[0] == ' ') val.remove_prefix(1);

      headers_.emplace(key, val);
    }
    line_start = line_end + 2;
  }
}
}