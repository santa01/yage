#version 330

#define MAX_LIGHTS          12
#define MAX_MATERIALS       16
#define M_PI                3.14159265358979323846
#define BIAS                0.00001

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

uniform vec3 cameraPosition;
uniform sampler2D diffuseTextureSampler;
uniform sampler2D specularTextureSampler;
uniform sampler2D shadowMapSamplers[MAX_LIGHTS];

smooth in vec3 fragmentPosition;
smooth in vec3 fragmentNormal;
smooth in vec2 fragmentUv;
smooth in vec4 lightSpaceFragmentPosition;

out vec4 fragmentColor;

const mat4 normalizedToUvMatrix = mat4(0.5f, 0.0f, 0.0f, 0.0f,
                                       0.0f, 0.5f, 0.0f, 0.0f,
                                       0.0f, 0.0f, 0.5f, 0.0f,
                                       0.5f, 0.5f, 0.5f, 1.0f);

vec3 getLightDirection(in LightSource lightSource) {
    switch (lightSource.type) {
        case TYPE_POINT:
            return normalize(fragmentPosition - lightSource.position);
        case TYPE_DIRECTED:
        case TYPE_SPOT:
            return lightSource.direction;
    }
}

float getLightAttenuation(in LightSource lightSource) {
    switch (lightSource.type) {
        case TYPE_DIRECTED:
            return 1.0f;
        case TYPE_POINT:
        case TYPE_SPOT:
            float falloffSquare = pow(lightSource.falloff, 2);
            float distanceSquare = pow(distance(lightSource.position, fragmentPosition), 2);
            return falloffSquare / (falloffSquare + distanceSquare);
    }
}

float getSpotFactor(in LightSource lightSource) {
    switch (lightSource.type) {
        case TYPE_DIRECTED:
        case TYPE_POINT:
            return 1.0f;
        case TYPE_SPOT:
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

            return 1.0f;
    }
}

float getShadowFactor(in LightSource lightSource) {
    if (lightSource.shadow) {
        switch (lightSource.type) {
            case TYPE_POINT:
            case TYPE_DIRECTED:
                return 0.0f;
            case TYPE_SPOT:
                vec4 ndcLightSpaceFragmentPosition = lightSpaceFragmentPosition / lightSpaceFragmentPosition.w;
                vec3 occludeFragmentPosition = (normalizedToUvMatrix * ndcLightSpaceFragmentPosition).xyz;
                float shadowMapDepth = texture(shadowMapSamplers[0], occludeFragmentPosition.st);

                if (occludeFragmentPosition.z + BIAS < shadowMapDepth) {
                    return 0.0f;
                }
        }
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
        vec4 diffuseColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
        vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

        vec3 direction = getLightDirection(light.sources[i]);
        float diffuseFactor = dot(normal, -direction);
        float spotFactor = getSpotFactor(light.sources[i]);
        float shadowFactor = getShadowFactor(light.sources[i]);

        if (diffuseFactor > 0) {
            float attenuation = getLightAttenuation(light.sources[i]);
            diffuseColor = vec4(light.sources[i].color, 1.0f) *
                           light.sources[i].energy * attenuation *
                           materialParameters.materials[0].diffuseIntensity *
                           diffuseFactor;

            vec3 viewerDirection = normalize(cameraPosition - fragmentPosition);
            vec3 reflectionDirection = normalize(reflect(direction, normal));
            float specularFactor = dot(viewerDirection, reflectionDirection);
            
            if (specularFactor > 0) {
                specularColor = vec4(light.sources[i].color, 1.0f) *
                                light.sources[i].energy * attenuation *
                                materialParameters.materials[0].specularIntensity *
                                pow(specularFactor, materialParameters.materials[0].specularExponent);
            }
        }

        lightColor += (diffuseColor + specularColor) * spotFactor * shadowFactor;
    }

    fragmentColor = diffuseTextureColor * (ambientColor + lightColor);
}
