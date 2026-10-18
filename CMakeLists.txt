
cmake_minimum_required(VERSION 3.24)

project(CrowdSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SFML_DIR "C:/DevTools/SFML-3.1.0/lib/cmake/SFML")

find_package(SFML 3 COMPONENTS Graphics Window System CONFIG REQUIRED)

add_executable(CrowdSim
    src/main.cpp
    src/Agent.cpp
    src/Agent.hpp
    src/Simulation.hpp
    src/Simulation.cpp
)

target_link_libraries(CrowdSim PRIVATE
    SFML::Graphics
)

if(WIN32)
    add_custom_command(TARGET CrowdSim POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:CrowdSim>
            $<TARGET_FILE_DIR:CrowdSim>
        COMMAND_EXPAND_LISTS
    )
endif()