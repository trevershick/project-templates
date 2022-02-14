# - Try to find Smatch
#
# Once done this will define
#  SMATCH_FOUND - System has Check
#  SMATCH_INCLUDE_DIRS - The Check include directories
#  SMATCH_LIBRARIES - The libraries needed to use Check

find_package(PkgConfig)

find_path(SMATCH_INCLUDE_DIR smatch.h)
#PATH_SUFFIXES smatch)

find_library(SMATCH_LIBRARY NAMES smatch )

set(SMATCH_LIBRARIES ${SMATCH_LIBRARY})
set(SMATCH_INCLUDE_DIRS ${SMATCH_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIET and REQUIRED arguments and set SMATCH_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(smatch DEFAULT_MSG
  SMATCH_LIBRARY SMATCH_INCLUDE_DIR)

mark_as_advanced(SMATCH_INCLUDE_DIR SMATCH_LIBRARY)
