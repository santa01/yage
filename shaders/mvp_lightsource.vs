#version 330

uniform mat4 mvp;
uniform mat4 mv;

layout(location = 0) in vec3 vertexPosition;

void main () {
    gl_Position = mvp * mv * vec4(vertexPosition, 1.0f);
}
