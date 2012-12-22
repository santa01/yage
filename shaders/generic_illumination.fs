#version 330
#extension GL_ARB_texture_cube_map_array: enable

#define MAX_LIGHTS          16
#define MAX_MATERIALS       16
#define M_PI                3.14159265358979323846

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

/* Dont change fields order! */
struct Masterial {
    float diffuseIntensity;
    float specularIntensity;
    float specularExponent;
};

layout(std140) uniform AmbientLight {
    vec3 color;
    float intensity;
} ambientLight;

// TODO: impelement in user space
layout(std140) uniform MeshParameters {
    int castsShadow;
    int receivesShadow;
} meshParameters;

layout(std140) uniform MaterialParameters {
    int materialsCount;
    Masterial materials[MAX_MATERIALS];
} materialParameters;

layout(std140) uniform Light {
    int sourcesCount;
    LightSource sources[MAX_LIGHTS];
} light;

const mat4 normalizedUvMatrix = mat4(0.5f, 0.0f, 0.0f, 0.0f,
                                     0.0f, 0.5f, 0.0f, 0.0f,
                                     0.0f, 0.0f, 0.5f, 0.0f,
                                     0.5f, 0.5f, 0.5f, 1.0f);

// This is a quirk to for proper cubemap sampling.
// For some reason cubemaps have Y flipped.
const mat3 invertYCoord = mat3(1.0f,  0.0f, 0.0f,
                               0.0f, -1.0f, 0.0f,
                               0.0f,  0.0f, 1.0f);

uniform vec3 cameraPosition;
uniform sampler2D diffuseTextureSampler;
uniform sampler2D specularTextureSampler;
uniform sampler2DArray shadowMapArraySampler;
uniform samplerCubeArray shadowCubeMapArraySampler;

smooth in vec3 fragmentPosition;
smooth in vec3 fragmentNormal;
smooth in vec2 fragmentUv;
smooth in vec4 lightSpaceFragmentPositions[MAX_LIGHTS];

out vec4 fragmentColor;

vec3 getLightDirection(in int lightIndex) {
    LightSource lightSource = light.sources[lightIndex];

    if (lightSource.type == TYPE_POINT) {
        return normalize(fragmentPosition - lightSource.position);
    }
    
    return lightSource.direction;
}

float getLightAttenuation(in int lightIndex) {
    LightSource lightSource = light.sources[lightIndex];

    if (lightSource.type == TYPE_DIRECTED) {
        return 1.0f;
    }

    float falloffSquare = pow(lightSource.falloff, 2);
    float distanceSquare = pow(distance(lightSource.position, fragmentPosition), 2);
    return falloffSquare / (falloffSquare + distanceSquare);
}

float getSpotFactor(in int lightIndex) {
    LightSource lightSource = light.sources[lightIndex];

    if (lightSource.type == TYPE_SPOT) {
        float viewAngle = pow(dot(lightSource.direction, normalize(fragmentPosition - lightSource.position)), 2.0f);
        float maxAngle = pow(cos(lightSource.size * M_PI / 360.0f), 2.0f);
        if (viewAngle < maxAngle) {
            return 0.0f;
       }

        float blend = 1.0f - lightSource.blend;
        float blendAngle = 1.0f / (blend / maxAngle - blend + 1.0f);
        if (viewAngle < blendAngle) {
           return (viewAngle - maxAngle) / (blendAngle - maxAngle);
        }
    }

    return 1.0f;
}

float getShadowFactor(in int lightIndex) {
    LightSource lightSource = light.sources[lightIndex];
    float shadowMapDepth = 0.0f;
    float fragmentDepth = 0.0f;

    if (lightSource.shadow) {
        if (lightSource.type == TYPE_POINT) {
            vec3 fragmentDirection = invertYCoord *
                    (fragmentPosition - lightSource.position);
            vec4 textureCoordinate = vec4(fragmentDirection, lightIndex);
            shadowMapDepth = texture(shadowCubeMapArraySampler,
                                     textureCoordinate).r;
            fragmentDepth = length(fragmentDirection) - 0.02f;
        } else {
            vec4 lightSpaceFragmentPosition = lightSpaceFragmentPositions[lightIndex];
            vec4 occludedFragmentPosition = normalizedUvMatrix *
                    (lightSpaceFragmentPosition / lightSpaceFragmentPosition.w);
            vec3 textureCoordinate = vec3(occludedFragmentPosition.st, lightIndex);
            shadowMapDepth = texture(shadowMapArraySampler,
                                     textureCoordinate).r;
            fragmentDepth = occludedFragmentPosition.z - 0.0002f;
        }
    }

    if (fragmentDepth > shadowMapDepth) {
        return 0.0f;
    }

    return 1.0f;
}

void main() {
    vec4 diffuseTextureColor = texture(diffuseTextureSampler, fragmentUv);
    vec3 normal = normalize(fragmentNormal);

    vec4 ambientColor = vec4(ambientLight.color, 1.0f) *
                        ambientLight.intensity;

    vec4 lightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < light.sourcesCount; i++) {
        LightSource lightSource = light.sources[i];
        
        float shadowFactor = getShadowFactor(i);
        if (shadowFactor > 0.0f) {
            vec4 diffuseColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
            vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

            vec3 direction = getLightDirection(i);
            float diffuseFactor = dot(normal, -direction);
            float spotFactor = getSpotFactor(i);

            if (diffuseFactor > 0) {
                float attenuation = getLightAttenuation(i);
                diffuseColor = vec4(lightSource.color, 1.0f) *
                               lightSource.energy *
                               attenuation *
                               materialParameters.materials[0].diffuseIntensity *
                               diffuseFactor;

                vec3 viewerDirection = normalize(cameraPosition - fragmentPosition);
                vec3 reflectionDirection = normalize(reflect(direction, normal));
                float specularFactor = dot(viewerDirection, reflectionDirection);

                if (specularFactor > 0) {
                    specularColor = vec4(lightSource.color, 1.0f) *
                                    lightSource.energy *
                                    attenuation *
                                    materialParameters.materials[0].specularIntensity *
                                    pow(specularFactor, materialParameters.materials[0].specularExponent);
                }
            }

            lightColor += (diffuseColor + specularColor) * spotFactor * shadowFactor;
        }
    }

    fragmentColor = diffuseTextureColor * (ambientColor + lightColor);
}
