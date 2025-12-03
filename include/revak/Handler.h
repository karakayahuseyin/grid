/**
 * @file Handler.h
 * @brief Handler function type declaration
 * 
 * Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
 * Licensed under the MIT License. Part of the Revak project.
 */

#include <functional>

namespace revak {

class Response;
class Request;

using Handler = std::function<Response(const Request&)>;

} // namespace revak