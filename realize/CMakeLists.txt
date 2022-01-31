cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

project(
  realize
  VERSION 1.0
  LANGUAGES C
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

if(MSVC)
  target_compile_options(realize PRIVATE /W4 /WX)
else()
  target_compile_options(realize PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter)
endif()

