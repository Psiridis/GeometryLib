# GeometryLib

[![Docs](https://github.com/Psiridis/GeometryLib/actions/workflows/docs.yml/badge.svg)](https://psiridis.github.io/GeometryLib/)

GeometryLib is a C++20 3D geometry library built with CMake. It provides analytic geometry primitives — points, vectors, lines, rays, and planes — a bounded shapes layer — segments, triangles, circles, and axis-aligned bounding boxes — and a spatial query layer covering intersection, parallelism, projection, and distance operations. The API is clean, well-tested, and uses scale-independent floating-point comparisons throughout.

The project exports an installable library target (`GeometryLib::Geometry`), ships CMake package configuration files for downstream `find_package` consumers, and uses GoogleTest for unit testing.

## Project layout

```text
Geometry/
├── CMakeLists.txt
├── CMakePresets.json
├── cmake/
│   ├── GeometryLibConfig.cmake.in
│   ├── GeometrySources.cmake        # lists all library source files
│   └── GeometryTestSources.cmake   # lists all test source files
├── include/Geometry/
│   ├── Primitives/   # point, vector, line, ray, plane
│   ├── Shapes/       # segment, triangle, circle, bounding_box
│   └── Queries/      # intersect, parallel, project, distance
├── src/              # mirrors include/ layout; also contains utils/
└── tests/            # mirrors include/ layout
```

## API documentation

The documentation is published automatically at:
**[https://psiridis.github.io/GeometryLib/](https://psiridis.github.io/GeometryLib/)**

It is regenerated on every push to `main` via the GitHub Actions workflow in `.github/workflows/docs.yml`.

To build the docs locally:

```bash
doxygen Doxyfile
open docs/html/index.html
```

## Requirements

- CMake 3.20 or newer
- A C++20-capable compiler
- Ninja
- Git when configuring a build with tests enabled, because GoogleTest is fetched at configure time
- Doxygen (optional — only required to build the documentation locally)

Notes:
- The provided presets use the Ninja generator explicitly.
- `ctest` is bundled with CMake and is used to run the test suite.

## Configure and build

The recommended workflow is to use the presets defined in `CMakePresets.json`.

### Debug build

The `debug` preset configures the project in `build-debug`, enables tests, and sets the install prefix to `install/` in the source tree.

```bash
cmake --preset debug
cmake --build --preset debug
```

### Release build

The `release` preset configures the project in `build-release` and disables tests.

```bash
cmake --preset release
cmake --build --preset release
```

### AddressSanitizer build

The `asan` preset configures the project in `build-asan`, enables tests, and adds AddressSanitizer flags.

```bash
cmake --preset asan
cmake --build --preset asan
```

### Reconfigure after CMake changes

If you change `CMakeLists.txt`, presets, or files included from CMake, rerun the configure step before building:

```bash
cmake --preset debug
cmake --build --preset debug
```

## Install the library

The install rules export the library target, public headers, and package configuration files.

### Install with a build preset

```bash
cmake --preset debug
cmake --build --preset install-debug
```

For release:

```bash
cmake --preset release
cmake --build --preset install-release
```

### Install from an existing build directory

```bash
cmake --install build-debug
```

By default, the presets install into:

```text
<project-root>/install
```

Installed package files are placed under:

```text
install/
├── include/
└── lib/cmake/GeometryLib/
```

## Run tests

Tests are enabled in the `debug` and `asan` presets and are built into a single executable named `geometry_tests`.

### Run tests with CTest

```bash
cmake --preset debug
cmake --build --preset debug --target geometry_tests
ctest --preset debug
```

The `debug` test preset already enables output on failure.

### Run the test executable directly

```bash
./build-debug/geometry_tests
```

If CTest reports `geometry_tests_NOT_BUILT`, reconfigure and rebuild the target:

```bash
cmake --preset debug
cmake --build --preset debug --target geometry_tests
ctest --preset debug
```

## Use the library from another CMake project

After installation, consumers can use the exported package configuration.

```cmake
find_package(GeometryLib CONFIG REQUIRED)

add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE GeometryLib::Geometry)
```

If GeometryLib is installed in a non-standard location, point CMake at the install prefix:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/Geometry/install
```

## Development notes

- Library implementation files are listed in `cmake/GeometrySources.cmake`.
- Test source files are listed in `cmake/GeometryTestSources.cmake`.
- GoogleTest is fetched only when `BUILD_TESTING` is enabled.
- The exported target name for consumers is `GeometryLib::Geometry`.
- `GEOM_ASSERT(condition)` fires before every `throw` in debug builds, providing an early abort with file and line info. Both mechanisms remain active — the assert for development, the throw for release-time error handling by callers.
- Floating-point comparisons use `almost_equal(a, b)` from `src/utils/numerical_utils.hpp`. Shared tolerance constants `k_abs_eps` (1e-12) and `k_rel_eps` (1e-9) are defined there and used consistently across all modules. All threshold checks in the query layer are scale-independent (squared ratios, no raw absolute distances). `safe_acos(x)` clamps its argument to [-1, 1] before calling `std::acos` to prevent NaN from floating-point rounding.