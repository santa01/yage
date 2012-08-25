#version 330

uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 lightSpaceMvp;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUv;

smooth out vec3 fragmentPosition;
smooth out vec3 fragmentNormal;
smooth out vec2 fragmentUv;
smooth out vec4 lightSpaceFragmentPosition;

void main () {
    fragmentPosition = (mv * vec4(vertexPosition, 1.0f)).xyz;
    fragmentNormal = (mv * vec4(vertexNormal, 0.0f)).xyz;
    fragmentUv = vertexUv;
    lightSpaceFragmentPosition = lightSpaceMvp * mv * vec4(vertexPosition, 1.0f);

    gl_Position = mvp * mv * vec4(vertexPosition, 1.0f);
}
