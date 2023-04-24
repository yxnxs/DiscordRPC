cmake_minimum_required (VERSION 3.18 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BUILD_SHARED_LIBS OFF)

option(BUILD_EXAMPLES "Build examples" ON)

set (project_name "DiscordRPC")
set (project_version "1.0.0")

project(${project_name} VERSION "${project_version}")
message(STATUS "Generating files for ${project_name} Version ${project_version}")

include(FetchContent)

FetchContent_Declare(
    djson
    GIT_REPOSITORY https://github.com/karnkaul/djson
    GIT_TAG 129c0b1
)

FetchContent_MakeAvailable(djson)

add_library(${project_name})
target_link_libraries(${project_name} djson)

add_subdirectory(src)
if (BUILD_EXAMPLES)
    add_subdirectory(example)
endif()