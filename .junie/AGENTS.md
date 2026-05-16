# Developer Guidelines for Cells Project

This document provides project-specific information for advanced developers to build, test, and develop the Cells engine.

### 1. Build and Configuration

The project uses CMake (minimum version 3.26) and supports [CMake Presets](CMakePresets.json).

#### Prerequisites
- **MSVC**: The project is primarily developed with MSVC on Windows.
- **Python 3**: Required for running code generation tools.
- **Jinja2**: Python library for code generation templates.

#### Build Instructions
1. **Configure the project**:
   Use one of the provided presets (e.g., `debug`, `release`):
   ```powershell
   cmake --preset debug
   ```
2. **Build a specific target**:
   Example for building the tests:
   ```powershell
   cmake --build build/debug --target cell-tests
   ```

#### Code Generation
The project uses ECS (Entity Component System) with some code-generated components and systems.
Before build, if you modify `.yaml` files in component directories, run the generation script:
```powershell
python tools/generate_code.py
```
This script uses Jinja2 templates located in `tools/templates` to generate headers and sources.

---

### 2. Testing

The project uses **GoogleTest**. The main test executable is `cell-tests`.

#### Running Tests
To run all tests:
```powershell
./build/debug/sources/tests/cell-tests.exe
```
To run specific tests (e.g., `ECS_TESTS`):
```powershell
./build/debug/sources/tests/cell-tests.exe --gtest_filter=ECS_TESTS.*
```

#### Adding New Tests
1. Create a new `.cpp` file in `sources/tests/`.
2. Register the file in `sources/tests/CMakeLists.txt` under `add_executable(cell-tests ...)`.
3. Use the standard GTest `TEST` or `TEST_F` macros.

#### Simple Test Example
```cpp
#include <gtest/gtest.h>

TEST(ExampleSuite, SimpleCalculation) {
    int result = 2 + 2;
    EXPECT_EQ(result, 4);
}
```

---

### 3. Development Information

#### Code Style
- **Indentation**: 4 spaces.
- **Braces**: K&R style (opening brace on the same line) for classes, methods, and control flow.
- **Naming Conventions**:
    - Member variables: Prefixed with an underscore (e.g., `_microseconds`).
    - Methods: PascalCase (e.g., `AsSeconds()`).
    - Namespaces: PascalCase (e.g., `Common`, `GameConfig`).
- **Modern C++**: The project uses C++23 (`cxx_std_23`). Trailing return types are used occasionally.
- **Asserts**: Use `ASSERT(condition, message)` for internal checks (defined in `common/asserts`).

#### ECS Architecture
- Uses **EnTT** as the underlying ECS registry.
- Components are often defined in `.yaml` files and auto-generated into `generated/` subdirectories.
- Logic is split into "Systems" that operate on the `entt::registry`.

#### Memory Management
- Prefer stack allocation and `std::unique_ptr`/`std::shared_ptr`.
- Use `GSL` (Guidelines Support Library) types like `gsl::span` where appropriate.
