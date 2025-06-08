#version 450

layout(location = 0) in vec3 vNormal;
layout(location = 1) in vec2 vUV;
layout(location = 2) in vec3 vViewDir;
layout(location = 0) out vec4 fragColor;

// Uniforms for base color, metallic, roughness
uniform sampler2D uAlbedoMap;
uniform sampler2D uMetallicMap;
uniform sampler2D uRoughnessMap;
uniform vec3 uLightDir;
uniform vec3 uLightColor;

void main() {
    vec3 albedo = texture(uAlbedoMap, vUV).rgb;
    float metallic = texture(uMetallicMap, vUV).r;
    float roughness = texture(uRoughnessMap, vUV).r;

    // Simplified PBR lighting (Cook-Torrance)
    vec3 normal = normalize(vNormal);
    vec3 lightDir = normalize(uLightDir);
    vec3 viewDir = normalize(vViewDir);

    float NdotL = max(dot(normal, lightDir), 0.0);

    // ... more PBR math here ...

    vec3 color = albedo * uLightColor * NdotL;
    fragColor = vec4(color, 1.0);
}