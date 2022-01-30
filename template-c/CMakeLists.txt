cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

project(
  PRJNAME
  VERSION 1.0
)

configure_file(src/PRJNAME_config.h.in PRJNAME_config.h)


file (
  GLOB_RECURSE PRJNAME_sources
  src/*c
  src/*.h
)

add_executable(
  PRJNAME
  ${PRJNAME_sources}
)

target_sources(
  PRJNAME
  PRIVATE
  ${PRJNAME_sources}
)

target_include_directories(
  PRJNAME
  PUBLIC
  "${PROJECT_BINARY_DIR}"
)

install(
  TARGETS PRJNAME
)
