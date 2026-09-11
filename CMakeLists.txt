cmake_minimum_required(VERSION 3.20)

project(
    DropletDiagnostics
    VERSION 0.1.0
    DESCRIPTION "Analysis framework for laser-microdroplet experiments"
    LANGUAGES CXX
)

# --------------------------------------------------
# Global C++ configuration
# --------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json.
# Very useful for clangd, VS Code, static analysis, etc.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --------------------------------------------------
# Droplet diagnostics library 
# --------------------------------------------------

add_library(droplet
    src/io/PGM.cpp
    src/io/TIFF.cpp
    src/Processing.cpp
    src/detection/Droplet_Detector.cpp
    src/Synthetic.cpp
)

target_include_directories(droplet PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_compile_features(droplet PUBLIC cxx_std_17)

# --------------------------------------------------
# Compiler warnings
# --------------------------------------------------

if(MSVC)
    target_compile_options(droplet PRIVATE /W4 /permissive-)
else()
    target_compile_options(droplet PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
    )
endif()

# --------------------------------------------------
# CLI application
# --------------------------------------------------

add_executable(droplet_cli
    apps/droplet_cli/main.cpp
)

target_link_libraries(droplet_cli PRIVATE droplet)

# Put executable in build/bin
set_target_properties(droplet_cli PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")