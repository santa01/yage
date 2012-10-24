#version 330

uniform mat4 modelViewPerspectiveMatrix;
uniform mat4 localWorldMatrix;

layout(location = 0) in vec3 vertexPosition;

smooth out vec3 fragmentPosition;

void main () {
    fragmentPosition = (localWorldMatrix * vec4(vertexPosition, 1.0f)).xyz;

    gl_Position = modelViewPerspectiveMatrix *
                  localWorldMatrix *
                  vec4(vertexPosition, 1.0f);
}
