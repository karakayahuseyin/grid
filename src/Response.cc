/**
 * @file Response.cc
 * @brief Response class implementation
 * 
 * Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
 * Licensed under the MIT License. Part of the Revak project.
 */

#include "revak/Response.h"

#include <format>

namespace revak {

void Response::SetStatus(int code) {
  status_code_ = code;
}

void Response::SetHeader(std::string key, std::string value) {
  headers_[std::move(key)] = std::move(value);
}

void Response::SetBody(std::string body) {
  body_ = std::move(body);
}

std::string Response::ToString() const {
  std::string status_message;
  switch (status_code_) {
  case 200: status_message = "OK"; break;
  case 201: status_message = "Created"; break;
  case 204: status_message = "No Content"; break;
  case 301: status_message = "Moved Permanently"; break;
  case 302: status_message = "Found"; break;
  case 304: status_message = "Not Modified"; break;
  case 401: status_message = "Unauthorized"; break;
  case 403: status_message = "Forbidden"; break;
  case 404: status_message = "Not Found"; break;
  case 405: status_message = "Method Not Allowed"; break;
  case 500: status_message = "Internal Server Error"; break;
  case 502: status_message = "Bad Gateway"; break;
  case 503: status_message = "Service Unavailable"; break;
  default:  status_message = "Unknown"; break;
  }

  std::string status_line = 
    std::format("HTTP/1.1 {} {}\r\n", status_code_, status_message);

  std::string headers;
  for (const auto& [key, val] : headers_) {
    headers += std::format("{}: {}\r\n", key, val);
  }
  headers += "\r\n";

  return status_line + headers + body_;
}

} // namespace revak