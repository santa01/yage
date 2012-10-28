#version 330

// This is a quirk to for proper cubemap sampling.
// For some reason cubemaps have Y flipped.
const mat3 invertYCoord = mat3(1.0f,  0.0f, 0.0f,
                               0.0f, -1.0f, 0.0f,
                               0.0f,  0.0f, 1.0f);

uniform vec3 cameraPosition;
uniform samplerCube diffuseTextureSampler;

smooth in vec3 fragmentPosition;

out vec4 fragmentColor;

void main() {
    vec3 fragmentDirection = invertYCoord * (fragmentPosition - cameraPosition);

    fragmentColor = texture(diffuseTextureSampler, fragmentDirection);
}
