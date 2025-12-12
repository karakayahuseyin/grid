/**
 * @file Response.cc
 * @brief Response class implementation
 * 
 * Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
 * Licensed under the MIT License. Part of the Revak project.
 */

#include "revak/Response.h"

#include <chrono>
#include <string>
#include <format>
#include <ctime>

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

int Response::GetStatusCode() const {
  return status_code_;
}

std::string Response::GetStatusText() const {
  switch (status_code_) {
  case 200: return "OK";
  case 201: return "Created";
  case 204: return "No Content";
  case 301: return "Moved Permanently";
  case 302: return "Found";
  case 304: return "Not Modified";
  case 401: return "Unauthorized";
  case 403: return "Forbidden";
  case 404: return "Not Found";
  case 405: return "Method Not Allowed";
  case 500: return "Internal Server Error";
  case 502: return "Bad Gateway";
  case 503: return "Service Unavailable";
  default:  return "Unknown";
  }
}

std::string Response::ToString() const {

  std::string status_line = 
    std::format("HTTP/1.1 {} {}\r\n", status_code_, GetStatusText());

  std::string headers;
  
  // Get current time in UTC for Date header
  auto now = std::chrono::system_clock::now();
  auto now_t = std::chrono::system_clock::to_time_t(now);
  
  // Convert to UTC time struct
  std::tm utc_time = *std::gmtime(&now_t);
  
  // Format RFC 7231 compliant Date header (UTC/GMT)
  char date_buffer[100];
  std::strftime(date_buffer, sizeof(date_buffer), "%a, %d %b %Y %H:%M:%S GMT", &utc_time);

  // Add Server and Date headers
  headers += "Server: Revak\r\n";
  headers += std::format("Date: {}\r\n", date_buffer);

  // Set Content-Length header only if not already set by user
  if (headers_.find("Content-Length") == headers_.end()) {
    headers += std::format("Content-Length: {}\r\n", body_.size());
  }
  
  for (const auto& [key, val] : headers_) {
    headers += std::format("{}: {}\r\n", key, val);
  }
  headers += "\r\n";
  return status_line + headers + body_;
}

} // namespace revak