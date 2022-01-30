cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

project(
  realize
  VERSION 1.0
)

configure_file(src/realize_config.h.in realize_config.h)


file (
  GLOB_RECURSE realize_sources
  src/*c
  src/*.h
)

add_executable(
  realize
  ${realize_sources}
)

if(MSVC)
  target_compile_options(realize PRIVATE /W4 /WX)
else()
  target_compile_options(realize PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter)
endif()

target_sources(
  realize
  PRIVATE
  ${realize_sources}
)

target_include_directories(
  realize
  PUBLIC
  "${PROJECT_BINARY_DIR}"
)

install(
  TARGETS realize
)
