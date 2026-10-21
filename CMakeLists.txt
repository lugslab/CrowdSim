
cmake_minimum_required(VERSION 3.24)

# ============================================================
# Project configuration
# ============================================================

project(CrowdSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# ============================================================
# Dependencies - SFML
# ============================================================

set(SFML_DIR "C:/DevTools/SFML-3.1.0/lib/cmake/SFML")

find_package(SFML 3 COMPONENTS Graphics Window System
    CONFIG REQUIRED
)


# ============================================================
# Core simulation library
#
# Shared by the main application and unit tests.
# ============================================================

add_library(CrowdSimCore STATIC
    src/Agent.cpp
    src/Agent.hpp
    src/DebugStats.hpp
    src/PerformanceMonitor.cpp
    src/PerformanceMonitor.hpp
    src/Personality.hpp
    src/Simulation.cpp
    src/Simulation.hpp
)

target_include_directories(CrowdSimCore PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(CrowdSimCore PUBLIC
    SFML::Graphics
)


# ============================================================
# Main application
# ============================================================

add_executable(CrowdSim
    src/main.cpp
    src/DebugPanel.cpp
    src/DebugPanel.hpp
)

target_link_libraries(CrowdSim PRIVATE
    CrowdSimCore
)


# ============================================================
# Runtime dependencies - Windows DLLs
# ============================================================

if(WIN32)
    add_custom_command(TARGET CrowdSim POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:CrowdSim>
            $<TARGET_FILE_DIR:CrowdSim>
        COMMAND_EXPAND_LISTS
        COMMENT "Copying runtime DLLs"
        VERBATIM
    )
endif()


# ============================================================
# Application assets
# ============================================================

add_custom_command(TARGET CrowdSim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:CrowdSim>/assets"
    COMMENT "Copying CrowdSim assets"
    VERBATIM
)


# ============================================================
# Dependencies - Google Test
# ============================================================

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# Ensure Google Test uses the same MSVC runtime library.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)


# ============================================================
# Unit tests
# ============================================================

enable_testing()

add_executable(Tests
    tests/AgentTests.cpp
    tests/SimulationTests.cpp
    tests/VectorUtilsTests.cpp
)

target_link_libraries(Tests PRIVATE
    CrowdSimCore
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(Tests)
