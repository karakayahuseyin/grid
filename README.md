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
- **Cross-platform Ready**: Currently Linux-focused with POSIX sockets

## Requirements

- **Compiler**: GCC 13+ or Clang 15+ with full C++20 support
- **Build System**: CMake 3.22 or higher
- **Operating System**: Linux (Ubuntu 20.04+, Fedora 35+, or similar)
- **Dependencies**: Standard library only (no external dependencies)

## Quick Start

### Build

```bash
mkdir build && cd build
CXX=g++-13 cmake ..
make
```

### Run

```bash
./bin/revak
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

## Usage Example

```cpp
#include "revak/Server.h"
#include "revak/Request.h"
#include "revak/Response.h"

int main() {
  try {
    // Create server on port 8080 with 4 worker threads
    revak::Server server(8080, 4);

    // Define routes
    server.Get("/hello", [](const revak::Request& req) {
      revak::Response res(200, "OK");
      res.SetHeader("Content-Type", "text/plain");
      res.SetBody("Hello, World!");
      return res;
    });

    server.Get("/user/:id", [](const revak::Request& req) {
      revak::Response res(200, "OK");
      res.SetBody("User ID: " + req.Path());
      return res;
    });

    // Start server (blocks until stopped)
    server.Run();
  } catch (const std::exception& e) {
    std::cerr << "Server error: " << e.what() << '\n';
    return 1;
  }
  return 0;
}
```

## Project Structure

```
revak/
├── include/revak/     # Public headers
│   ├── Server.h       # Main server orchestration
│   ├── Router.h       # Route matching and dispatch
│   ├── Request.h      # HTTP request parser
│   ├── Response.h     # HTTP response builder
│   ├── Socket.h       # POSIX socket wrapper
│   ├── ThreadPool.h   # Worker thread management
│   └── Handler.h      # Handler type alias
├── src/               # Implementation files
├── build/             # Build artifacts (generated)
└── main.cc            # Example application
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

## Development

### Code Style

This project follows modern C++ best practices:
- Const-correctness throughout the codebase
- Move semantics for efficiency
- `std::string_view` for non-owning string references
- RAII for all resource management
- Clear separation between interface and implementation

### Current Limitations

- No HTTPS/TLS support
- Basic routing (linear search, no path parameters extraction)
- No request body size limits
- No timeout management
- Blocking I/O (no async/await)

These are intentional for educational clarity and may be addressed in future versions.

## Roadmap

See the [Issues](https://github.com/karakayahuseyin/revak/issues) page for planned features and improvements:

- [ ] Testing infrastructure (Google Test)
- [ ] CI/CD pipeline (GitHub Actions)
- [ ] Code quality tools (clang-format, clang-tidy)
- [ ] Comprehensive documentation (Doxygen)
- [ ] Router optimization with map-based lookup
- [ ] Logging system
- [ ] Static file serving with MIME type detection

## Contributing

This is an educational project. Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

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
