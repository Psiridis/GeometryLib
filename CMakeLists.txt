cmake_minimum_required(VERSION 3.20)
project(GeometryLib VERSION 1.0 LANGUAGES CXX)

include(GNUInstallDirs)

include(cmake/GeometrySources.cmake)

add_library(Geometry)
# Namespaced alias for consumers: GeometryLib::Geometry
add_library(GeometryLib::Geometry ALIAS Geometry)

target_sources(Geometry PRIVATE
    ${GEOMETRY_SOURCES}
)

target_include_directories(Geometry
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(Geometry PUBLIC cxx_std_20)

# -------------------
# Install rules
# -------------------

# Install the library
install(TARGETS Geometry
    EXPORT GeometryLibTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}      # static libs
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}      # shared libs
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}      # windows dlls
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install public headers (everything under include/)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Export targets file (GeometryLibTargets.cmake)
install(EXPORT GeometryLibTargets
    FILE GeometryLibTargets.cmake
    NAMESPACE GeometryLib::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GeometryLib
)

# Generate and install Config + ConfigVersion files
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/GeometryLibConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/GeometryLibConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GeometryLib
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/GeometryLibConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/GeometryLibConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/GeometryLibConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GeometryLib
)

# -------------------
# Tests (optional)
# -------------------
include(CTest)

if(BUILD_TESTING)
    include(FetchContent)
    include(cmake/GeometryTestSources.cmake)

    if(NOT DEFINED INSTALL_GTEST)
        set(INSTALL_GTEST OFF CACHE BOOL "Disable googletest install target in this build")
    endif()

    if(MSVC AND NOT DEFINED gtest_force_shared_crt)
        set(gtest_force_shared_crt ON CACHE BOOL "Use shared CRT for googletest on MSVC")
    endif()

    # Pin to a specific version tag for deterministic test dependency resolution.
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(googletest)

    add_executable(geometry_tests)
    target_sources(geometry_tests PRIVATE
        ${GEOMETRY_TEST_SOURCES}
    )
    target_link_libraries(geometry_tests PRIVATE Geometry GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(geometry_tests)

endif()