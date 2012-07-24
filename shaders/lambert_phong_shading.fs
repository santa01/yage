#version 330

#define MAX_LIGHTS          16
#define M_PI                3.14159265358979323846

#define TYPE_DIRECTED       0
#define TYPE_POINT          1
#define TYPE_SPOT           2

struct LightSource {
    int type;
    float energy;
    float falloff;
    float size;
    vec3 color;
    float blend;
    vec3 position;
    vec3 direction;
};

layout(std140) uniform MaterialParameters {
    float diffuseIntensity;
    float specularIntensity;
    float specularExponent;
} materialParameters;

layout(std140) uniform AmbientLight {
    vec3 color;
    float intensity;
} ambientLight;

layout(std140) uniform Light {
    int sourcesCount;
    LightSource sources[MAX_LIGHTS];
} light;

uniform vec3 cameraPosition;
uniform sampler2D diffuseTextureSampler;

smooth in vec3 fragmentPosition;
smooth in vec3 fragmentNormal;
smooth in vec2 fragmentUv;

out vec4 fragmentColor;

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

void main() {
    vec4 diffuseTextureColor = texture(diffuseTextureSampler, fragmentUv.st);
    vec3 normal = normalize(fragmentNormal);

    vec4 ambientColor = vec4(ambientLight.color, 1.0f) *
                        ambientLight.intensity;

    vec4 lightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < light.sourcesCount; i++) {
        vec4 diffuseColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
        vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

        vec3 direction = getLightDirection(light.sources[i]);
        float spotFactor = getSpotFactor(light.sources[i]);
        float diffuseFactor = dot(normal, -direction);

        if (diffuseFactor > 0) {
            float attenuation = getLightAttenuation(light.sources[i]);
            diffuseColor = vec4(light.sources[i].color, 1.0f) *
                           light.sources[i].energy * attenuation *
                           materialParameters.diffuseIntensity *
                           diffuseFactor;

            vec3 viewerDirection = normalize(cameraPosition - fragmentPosition);
            vec3 reflectionDirection = normalize(reflect(direction, normal));
            float specularFactor = dot(viewerDirection, reflectionDirection);
            
            if (specularFactor > 0) {
                specularColor = vec4(light.sources[i].color, 1.0f) *
                                light.sources[i].energy * attenuation *
                                materialParameters.specularIntensity *
                                pow(specularFactor, materialParameters.specularExponent);
            }
        }

        lightColor += (diffuseColor + specularColor) * spotFactor;
    }

    fragmentColor = diffuseTextureColor * (ambientColor + lightColor);
}
