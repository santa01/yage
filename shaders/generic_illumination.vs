#version 330

#define MAX_LIGHTS          24

#define TYPE_DIRECTED       0
#define TYPE_POINT          1
#define TYPE_SPOT           2

/* Dont change fields order! */
struct LightSource {
    int type;
    float energy;
    float falloff;
    float size;
    vec3 color;
    float blend;
    vec3 position;
    bool shadow;
    vec3 direction;
};

layout(std140) uniform Light {
    int sourcesCount;
    LightSource sources[MAX_LIGHTS];
} light;

uniform mat4 modelViewPerspectiveMatrix;
uniform mat4 localWorldMatrix;
uniform mat4 lightModelViewPerspectiveMatrices[MAX_LIGHTS];

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUv;

smooth out vec3 fragmentPosition;
smooth out vec3 fragmentNormal;
smooth out vec2 fragmentUv;
smooth out vec4 lightSpaceFragmentPositions[MAX_LIGHTS];

void main () {
    fragmentPosition = (localWorldMatrix * vec4(vertexPosition, 1.0f)).xyz;
    fragmentNormal = (localWorldMatrix * vec4(vertexNormal, 0.0f)).xyz;
    fragmentUv = vertexUv;

    for (int i = 0; i < light.sourcesCount; i++) {
        if (light.sources[i].shadow) {
            switch (light.sources[i].type) {
                case TYPE_POINT:
                    break;

                default:
                    lightSpaceFragmentPositions[i] =
                            lightModelViewPerspectiveMatrices[i] *
                            localWorldMatrix *
                            vec4(vertexPosition, 1.0f);
            }
        }
    }

    gl_Position = modelViewPerspectiveMatrix *
                  localWorldMatrix *
                  vec4(vertexPosition, 1.0f);
}
