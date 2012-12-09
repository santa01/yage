# Try to find yage development files
#  YAGE_FOUND           - all files are in place
#  YAGE_HEADERS_FOUND   - headers are in place
#  YAGE_SRC_FOUND       - sources are in place
#  YAGE_MISC_FOUND      - other files are in place (configs, resources) (TODO)
#
#  YAGE_INCLUDE_DIR     - the yage include directory
#  YAGE_SRC             - the yage source files
#  YAGE_HEADERS         - the yage header files

set(YAGE_DIR "${PROJECT_SOURCE_DIR}")

aux_source_directory("${YAGE_DIR}/src" YAGE_SRC)
set(YAGE_SRC_FOUND "TRUE")

file(GLOB YAGE_HEADERS "${YAGE_DIR}/include/*.h")
set(YAGE_INCLUDE_DIR "${YAGE_DIR}/include")
set(YAGE_HEADERS_FOUND "TRUE")

set(YAGE_MISC_FOUND "TRUE")

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set YAGE_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(yage DEFAULT_MSG
    YAGE_DIR
    YAGE_HEADERS_FOUND
    YAGE_SRC_FOUND
    YAGE_MISC_FOUND
)

mark_as_advanced(
    YAGE_HEADERS_FOUND
    YAGE_HEADERS
    YAGE_INCLUDE_DIR
    YAGE_SRC_FOUND
    YAGE_SRC
    YAGE_MISC_FOUND
)
