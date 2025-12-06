# Revak
A high-performance HTTP 1.1 server running on Linux, using modern C++ 20 concepts.

## Features

- **Multithreaded Architecture**
- **Modern C++20 Design**
- **Simple API**

## Build & Run

Make sure you have a compiler version of gcc that supports all the features of C++20.

```bash
mkdir build && cd build
CXX=g++-13 cmake ..
make
./bin/revak
```

## Test

```bash
curl -v http://localhost:8080
```

## License

MIT License
