cmake_minimum_required(VERSION 2.8)

set(PROJECT yage)
project(${PROJECT} C CXX)

set(PROJECT_VERSION_MAJOR "0")
set(PROJECT_VERSION_MINOR "0")
set(PROJECT_VERSION_PATCH "1")
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(PROJECT_SUMMARY "Yet Another Game Engine")

if(${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(
        FATAL_ERROR "Building in source root is forbidden.\n"
        "Try to build in another directory e.g.\n"
        "${PROJECT_SOURCE_DIR}/build"
    )
endif(${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

# look for dependencies
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH})

# check C++11 features
include(CheckCXX11Features)
set(YAGE_CXX11_FLAGS "${CXX11_FLAGS}" CACHE STRING "Flags used to enable C++11 for compiler")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${YAGE_CXX11_FLAGS}")

find_package(X11 REQUIRED X11 Xrandr)
set(INCLUDE ${INCLUDE} ${X11_INCLUDE_DIR})
set(LIBRARIES ${LIBRARIES} ${X11_X11_LIB})

find_package(OpenGL REQUIRED gl)
set(INCLUDE ${INCLUDE} ${OPENGL_INCLUDE_DIR})
set(YAGE_OPENGL ${OPENGL_gl_LIBRARY} CACHE FILEPATH "OpenGL library used for building yage (useful for bumblebee users)")
set(LIBRARIES ${LIBRARIES} ${YAGE_OPENGL})

# find own sources
find_package(yage REQUIRED)

# TODO check required includes and configure variables
# include(CheckIncludeFileCXX)
# check_include_file_cxx("X11/extensions/Xrandr.h" HAVE_XRANDR_H)

# where to search sources and headers
set(INCLUDE ${INCLUDE} ${CMAKE_BINARY_DIR})
include_directories(${INCLUDE})
# override system includes (for OpenGL headers)
include_directories(BEFORE SYSTEM ${YAGE_INCLUDE_DIR})

# configure some headers
set(HAVE_XRANDR_H ${X11_Xrandr_FOUND})
configure_file(global.h.in global.h) # preprocess global.h.in header

# enable build-time warnings
set(ENABLE_WARNINGS TRUE CACHE BOOL "Enable build-time warnings")
if(ENABLE_WARNINGS)
    if(MSVC)
        # Force to always compile with W4
        if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
            string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
        endif()
    elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        # Update if necessary
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
    endif()
endif()

# yage library
add_library(${PROJECT} SHARED ${YAGE_SRC} ${YAGE_HEADERS})
target_link_libraries(${PROJECT} ${LIBRARIES})

# TODO: remove executable
link_directories("${CMAKE_INSTALL_PREFIX}/lib")
set(EXE run_${PROJECT})
set(EXE_SRC src/main.cpp)
add_executable(${EXE} ${EXE_SRC})
target_link_libraries(${EXE} ${PROJECT})

# Installation targets
install(TARGETS ${PROJECT} DESTINATION lib)
install(FILES ${YAGE_HEADERS} DESTINATION include/${PROJECT})

# Documenation
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(HAVE_DOT "NO")
    set(GENERATE_QHP "NO")
    option(MAKE_API_DOCS "Generate API documentation (doxygen)" ON)

    if(DOXYGEN_DOT_FOUND)
        set(HAVE_DOT "YES")
    endif(DOXYGEN_DOT_FOUND)

    find_program(QHELPGENERATOR_EXECUTABLE qhelpgenerator)
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(QHELPGENERATOR DEFAULT_MSG QHELPGENERATOR_EXECUTABLE)
    set(GENERATE_QHP ${QHELPGENERATOR_FOUND})

    configure_file(Doxyfile.in Doxyfile @ONLY)
endif(DOXYGEN_FOUND)

if(MAKE_API_DOCS)
    make_directory("${CMAKE_BINARY_DIR}/doc")
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_BINARY_DIR}/Doxyfile"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(MAKE_API_DOCS)

# CTest
option(MAKE_TESTS "Build tests with application" OFF)
if(${MAKE_TESTS})
    set(TESTS
        sample
    )
    enable_testing()
    foreach(TEST ${TESTS})
        add_subdirectory(tests/${TEST} tests/${TEST})
        add_test(NAME ${TEST} COMMAND tests/${TEST}/${TEST})
    endforeach(TEST ${TESTS})
endif(${MAKE_TESTS})

# CPack
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME ${PROJECT})
set(CPACK_PACKAGE_VENDOR "YAGE Project")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_SUMMARY})

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Nobody")
set(CPACK_DEBIAN_PACKAGE_SECTION "games")

set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/COPYING")

set(CPACK_STRIP_FILES "TRUE")
set(CPACK_GENERATOR
    "STGZ"
    "TGZ"
    "ZIP"
    "DEB" # requires dpkg
#     "RPM" # requires rpmbuild
#     "NSIS" # Nullsoft installer
)
set(CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_PROCESSOR})
include(CPack)
