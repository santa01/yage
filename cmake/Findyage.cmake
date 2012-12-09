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

set(YAGE_SRC
    src/RenderEffect.cpp
    src/TGAImage.cpp
    src/Vec3.cpp
    src/Image.cpp
    src/Mat4.cpp
    src/Rotatable.cpp
    src/Camera.cpp
    src/Scene.cpp
    src/DirectedLight.cpp
    src/Scalable.cpp
    src/Quaternion.cpp
    src/Texture.cpp
    src/Frustum.cpp
    src/Extention.cpp
    src/Mat3.cpp
    src/Game.cpp
    src/Movable.cpp
    src/Renderable.cpp
    src/Light.cpp
    src/Mesh.cpp
    src/OpenGLWindow.cpp
    src/Renderer.cpp
    src/Logger.cpp
    src/SpotLight.cpp
    src/ShaderLoader.cpp
    src/PointLight.cpp
    src/Vec4.cpp
    src/Config.cpp
)
set(YAGE_SRC_FOUND "TRUE")

set(YAGE_HEADERS
    include/Rotatable.h
    include/Mat4.h
    include/configuration.h
    include/TGAImage.h
    include/Image.h
    include/Logger.h
    include/Mesh.h
    include/Config.h
    include/Mat3.h
    include/Vec4.h
    include/Scalable.h
    include/Light.h
    include/SpotLight.h
    include/OpenGLWindow.h
    include/Movable.h
    include/DirectedLight.h
    include/Quaternion.h
    include/PointLight.h
    include/Frustum.h
    include/Camera.h
    include/RenderEffect.h
    include/Renderer.h
    include/Vec3.h
    include/Scene.h
    include/Extention.h
    include/Texture.h
    include/Game.h
    include/Renderable.h
    include/ShaderLoader.h
)
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
