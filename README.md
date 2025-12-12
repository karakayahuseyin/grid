# Revak

<p align="center">
  <img src="docs/assets/revak.png" alt="Revak Logo" width="200"/>
</p>

A lightweight, high-performance HTTP/1.1 server built with modern C++20, designed for educational purposes to demonstrate clean server architecture and best practices.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Overview

Revak is a multithreaded HTTP/1.1 server that showcases modern C++ design patterns including RAII, composition, and efficient thread pool management. The project emphasizes code clarity, const-correctness, and adherence to HTTP standards (RFC 7231).

## Features

- **HTTP/1.1 Compliance**: Proper request parsing, response formatting, and standard headers (Date, Server, Content-Length)
- **Multithreaded Architecture**: Efficient thread pool for concurrent request handling
- **Express-like Routing**: Simple, intuitive API for defining routes with HTTP methods
- **Modern C++20**: Leverages concepts, string_view, and move semantics for optimal performance
- **RAII Socket Management**: Automatic resource cleanup with proper error handling
- **Asynchronous Logging**: Non-blocking logging mechanism to avoid performance bottlenecks
- **Cross-platform Ready**: Currently Linux-focused with POSIX sockets

## Requirements

- **Compiler**: GCC 13+ or Clang 15+ with full C++20 support
- **Build System**: CMake 3.10 or higher
- **Operating System**: Linux
- **Dependencies**: Standard library only (no external dependencies)

## Quick Start

### Build

```bash
cmake -B build
cmake --build build
```

### Run

```bash
./build/bin/revak
```

The server starts on `http://localhost:8080` by default.

### Test

```bash
# Simple GET request
curl http://localhost:8080/hello

# Verbose output to see headers
curl -v http://localhost:8080/hello

# Using netcat for raw HTTP
echo -e "GET /hello HTTP/1.1\r\nHost: localhost\r\n\r\n" | nc localhost 8080
```

## Architecture

**Revak** follows a layered architecture:

1. **Socket Layer**: RAII wrapper around POSIX TCP sockets with SO_REUSEADDR for development convenience
2. **HTTP Layer**: Request parsing and Response formatting with automatic header management
3. **Routing Layer**: Pattern matching and handler dispatch with method-based routing
4. **Server Layer**: Orchestrates socket, thread pool, and router with graceful shutdown support

**Key Design Patterns**:
- **RAII**: Automatic resource management (sockets, threads)
- **Composition**: Server composes Socket, ThreadPool, and Router
- **Callback Pattern**: User-defined handlers as std::function
- **Thread Pool Pattern**: Fixed worker threads process requests from job queue

## Current Limitations

- No HTTPS/TLS support
- Basic routing (linear search, no path parameters extraction)
- No request body size limits
- No timeout management
- Blocking I/O (no async/await)

These are intentional for educational clarity and may be addressed in future versions.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

Built as an educational project to explore:
- HTTP/1.1 protocol implementation
- Modern C++20 features and idioms
- Multithreaded server design patterns
- Clean, maintainable C++ architecture

---

**Note**: This server is designed for learning and development purposes. It is not recommended for production use without significant security hardening and feature additions.
