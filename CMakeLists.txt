cmake_minimum_required(VERSION 2.8)

set(PROJECT yage)
project(${PROJECT} C CXX)

if(${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(
    FATAL_ERROR "Building in source root is forbidden.\n"
    "Try to build in another directory e.g.\n"
    ${PROJECT_SOURCE_DIR} "/build"
  )
endif(${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

find_package(X11 REQUIRED X11 Xrandr)
if(X11_FOUND)
    set(INCLUDE ${INCLUDE} ${X11_INCLUDE_DIR})
    set(LIBRARIES ${LIBRARIES} ${X11_X11_LIB})
endif(X11_FOUND)

find_package(OpenGL REQUIRED gl)
if(OPENGL_FOUND)
    set(INCLUDE ${INCLUDE} ${OPENGL_INCLUDE_DIR})
    set(YAGE_OPENGL ${OPENGL_gl_LIBRARY} CACHE FILEPATH "OpenGL library used for building yage (useful for bumblebee users)")
    set(LIBRARIES ${LIBRARIES} ${YAGE_OPENGL})
endif(OPENGL_FOUND)

# TODO: custom modules
# set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules/" ${CMAKE_MODULE_PATH})

# check required includes and configure variables
# include(CheckIncludeFileCXX)
# check_include_file_cxx("X11/extensions/Xrandr.h" HAVE_XRANDR_H)

# configure project files
set(SRC
    src/RenderEffect.cpp        src/TGAImage.cpp        src/Vec3.cpp
    src/Image.cpp               src/Mat4.cpp            src/Rotatable.cpp
    src/Camera.cpp              src/Scene.cpp           src/DirectedLight.cpp
    src/Scalable.cpp            src/Quaternion.cpp      src/Texture.cpp
    src/Frustum.cpp             src/Extention.cpp       src/Mat3.cpp
    src/Game.cpp                src/Movable.cpp         src/Renderable.cpp
    src/Light.cpp               src/Mesh.cpp            src/OpenGLWindow.cpp
    src/Renderer.cpp            src/Logger.cpp          src/SpotLight.cpp
    src/ShaderLoader.cpp        src/PointLight.cpp      src/Vec4.cpp
    src/Config.cpp
)
# aux_source_directory(src SRC)

set(HEADERS
    include/Rotatable.h         include/Mat4.h          include/configuration.h
    include/TGAImage.h          include/Image.h         include/Logger.h
    include/Mesh.h              include/Config.h        include/Mat3.h
    include/Vec4.h              include/Scalable.h      include/Light.h
    include/SpotLight.h         include/OpenGLWindow.h  include/Movable.h
    include/DirectedLight.h     include/Quaternion.h    include/PointLight.h
    include/Frustum.h           include/Camera.h        include/RenderEffect.h
    include/Renderer.h          include/Vec3.h          include/Scene.h
    include/Extention.h         include/Texture.h       include/Game.h
    include/Renderable.h        include/ShaderLoader.h
)

# where to search sources and headers
set(INCLUDE ${INCLUDE} "${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/include" ${CMAKE_BINARY_DIR})

set(FOO "Bar")
set(HAVE_XRANDR_H ${X11_Xrandr_FOUND})
configure_file(config.h.in config.h) # preprocess config.h.in header

add_library(${PROJECT} SHARED ${SRC} ${HEADERS})
include_directories(${INCLUDE})
target_link_libraries(${PROJECT} ${LIBRARIES})

link_directories("${CMAKE_INSTALL_PREFIX}/lib")
set(EXE run_${PROJECT})
set(EXE_SRC src/main.cpp)
add_executable(${EXE} ${EXE_SRC})
target_link_libraries(${EXE} ${PROJECT})

install(TARGETS ${PROJECT} DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/${PROJECT})

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
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "YAGE - Yet Another Game Engine")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Nobody")
set(CPACK_DEBIAN_PACKAGE_SECTION "games")

set(CPACK_PACKAGE_VERSION "0.0.1")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "1")

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
