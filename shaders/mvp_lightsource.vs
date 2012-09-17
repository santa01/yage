#version 330

uniform mat4 modelViewPerspectiveMatrix;
uniform mat4 localWorldMatrix;

layout(location = 0) in vec3 vertexPosition;

void main () {
    gl_Position = modelViewPerspectiveMatrix *
                  localWorldMatrix *
                  vec4(vertexPosition, 1.0f);
}
