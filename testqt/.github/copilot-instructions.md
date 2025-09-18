# Copilot Instructions for AI Agents

## Project Overview
This project demonstrates custom smart pointer implementations in C++ (unique/shared) and their usage, with a focus on memory management, ownership semantics, and multithreading. It uses Qt Core for the application entry point but is otherwise standard C++17.

### Key Components
- `myunique_ptr.h`: Custom unique pointer implementation (move-only, single ownership)
- `myshared_ptr.h`: Custom shared pointer implementation (reference counting, thread-safe)
- `shared_ptr_test.cpp`, `unique_ptr_test.cpp`, `weak_ptr_test.cpp`: Example/test code for pointer types, including multithreaded scenarios and cyclic reference demonstration
- `general.cpp`: Miscellaneous C++ idioms, STL usage, and lambda examples
- `main.cpp`: Entry point, runs selected pointer tests

## Build & Run
- **Build:** Use the VS Code task `Build cpp_recall` (runs CMake in `build/Desktop_Qt_6_9_1-Debug`)
- **Run:** Use the VS Code task `Build and Run cpp_recall` (builds and then runs the binary)
- **Manual:**
  1. `cd build/Desktop_Qt_6_9_1-Debug`
  2. `cmake ../.. && cmake --build .`
  3. `./cpp_recall`

## Project Conventions
- All custom smart pointer logic is in header files (`myunique_ptr.h`, `myshared_ptr.h`).
- Test/usage code is in separate `*_test.cpp` files, invoked from `main.cpp`.
- Prefer `myshared_ptr` and `myunique_ptr` over STL equivalents in tests to exercise custom logic.
- Use `std::atomic` for thread-safe reference counting in `myshared_ptr`.
- Qt is only used for the application entry point (`QCoreApplication`).
- Multithreaded tests (see `shared_ptr_test.cpp`) use `std::thread` and `std::mutex`.
- Cyclic reference and weak pointer concepts are demonstrated in `weak_ptr_test.cpp` (uses STL smart pointers for this part).

## Patterns & Examples
- To add a new smart pointer test, create a new `*_test.cpp` and call it from `main.cpp`.
- For thread safety, always use atomic operations for reference counts.
- For memory leak/cyclic reference demonstration, see `weak_ptr_test.cpp` and the `test_cyclic_graph` function.

## External Dependencies
- Requires Qt (Core module) for entry point only.
- C++17 required (see `CMakeLists.txt`).

## Key Files
- `myunique_ptr.h`, `myshared_ptr.h`, `shared_ptr_test.cpp`, `unique_ptr_test.cpp`, `weak_ptr_test.cpp`, `main.cpp`, `CMakeLists.txt`

---
For more details, see comments in the respective files. When in doubt, follow the patterns in the test files and keep new logic modular and well-commented.
