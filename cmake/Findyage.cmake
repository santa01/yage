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
    src/Camera.cpp
    src/Config.cpp
    src/CubeTexture.cpp
    src/DirectedLight.cpp
    src/Extention.cpp
    src/FrameBuffer.cpp
    src/Game.cpp
    src/Image.cpp
    src/ImageTexture.cpp
    src/Light.cpp
    src/Logger.cpp
    src/main.cpp
    src/Mat3.cpp
    src/Mat4.cpp
    src/Mesh.cpp
    src/Movable.cpp
    src/OpenGLWindow.cpp
    src/OrthographicProjection.cpp
    src/PerspectiveProjection.cpp
    src/PointLight.cpp
    src/Projection.cpp
    src/Quaternion.cpp
    src/Renderable.cpp
    src/RenderEffect.cpp
    src/Renderer.cpp
    src/Rotatable.cpp
    src/Scalable.cpp
    src/Scene.cpp
    src/ShaderLoader.cpp
    src/ShadowCubeMapArray.cpp
    src/ShadowMapArray.cpp
    src/SkyBox.cpp
    src/SpotLight.cpp
    src/Texture.cpp
    src/TGAImage.cpp
    src/Vec3.cpp
    src/Vec4.cpp
)
set(YAGE_SRC_FOUND "TRUE")

set(YAGE_HEADERS
    include/Camera.h
    include/Config.h
    include/configuration.h
    include/CubeTexture.h
    include/DirectedLight.h
    include/Extention.h
    include/FrameBuffer.h
    include/Game.h
    include/Image.h
    include/ImageTexture.h
    include/Light.h
    include/Logger.h
    include/Mat3.h
    include/Mat4.h
    include/Mesh.h
    include/Movable.h
    include/OpenGLWindow.h
    include/OrthographicProjection.h
    include/PerspectiveProjection.h
    include/PointLight.h
    include/Projection.h
    include/Quaternion.h
    include/Renderable.h
    include/RenderEffect.h
    include/Renderer.h
    include/Rotatable.h
    include/Scalable.h
    include/Scene.h
    include/ShaderLoader.h
    include/ShadowCubeMapArray.h
    include/ShadowMapArray.h
    include/SkyBox.h
    include/SpotLight.h
    include/Texture.h
    include/TGAImage.h
    include/Vec3.h
    include/Vec4.h
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
