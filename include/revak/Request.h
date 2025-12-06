/**
 * @file Request.h
 * @brief Request class declaration
 * 
 * Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
 * Licensed under the MIT License. Part of the Revak project.
 */

#pragma once

#include <string>
#include <string_view>
#include <map>

namespace revak {

class Request {
public:
  Request(const std::string_view& request);

  const std::string& Method() const {return method_;}
  const std::string& Path() const {return path_;}
  const std::string& Body() const {return body_;}

private:
  std::string method_;
  std::string path_;
  std::map<std::string, std::string> headers_;
  std::string body_;
};

} // namespace revak