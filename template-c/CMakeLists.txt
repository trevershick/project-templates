cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(PRJNAME)

configure_file(src/PRJNAME_config.h.in PRJNAME_config.h)

add_executable(
  PRJNAME
  src/PRJNAME.c
)

file (
  GLOB_RECURSE
  PRJNAME_sources
  src/**.*
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

