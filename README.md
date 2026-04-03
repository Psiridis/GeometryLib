# GeometryLib

GeometryLib is a C++20 3D geometry library built with CMake. It provides analytic geometry primitives — points, vectors, lines, rays, and planes — with a clean, well-tested API and correct floating-point comparisons throughout.

The project exports an installable library target (`GeometryLib::Geometry`), ships CMake package configuration files for downstream `find_package` consumers, and uses GoogleTest for unit testing.

## Project layout

```text
Geometry/
├── CMakeLists.txt
├── CMakePresets.json
├── cmake/
│   ├── GeometryLibConfig.cmake.in
│   ├── GeometrySources.cmake
│   └── GeometryTestSources.cmake
├── include/Geometry/
│   └── Primitives/
│       ├── point.hpp        # 3D point — distance, equality, arithmetic with Vector
│       ├── vector.hpp       # 3D vector — length, normalize, dot, cross, angle, arithmetic
│       ├── line.hpp         # Infinite line — origin + direction, parametric at(t)
│       ├── ray.hpp          # Semi-infinite ray — origin + direction, at(t) for t ≥ 0
│       └── plane.hpp        # Infinite plane — point + unit normal, signed distance, contains()
├── src/
│   ├── Primitives/
│   │   ├── point.cpp
│   │   ├── vector.cpp
│   │   ├── line.cpp
│   │   ├── ray.cpp
│   │   └── plane.cpp
│   └── utils/
│       ├── numerical_utils.hpp   # almost_equal(a, b, abs_eps, rel_eps)
│       ├── numerical_utils.cpp
│       └── assert_utils.hpp      # GEOM_ASSERT — aborts in debug, no-op in release
└── tests/
    └── Primitives/
        ├── test_point.cpp
        ├── test_vector.cpp
        ├── test_line.cpp
        ├── test_ray.cpp
        └── test_plane.cpp
```

## Requirements

- CMake 3.20 or newer
- A C++20-capable compiler
- Ninja
- Git when configuring a build with tests enabled, because GoogleTest is fetched at configure time

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
- Floating-point equality uses a combined absolute + relative epsilon comparison (`almost_equal`) defined in `src/utils/numerical_utils.hpp`.