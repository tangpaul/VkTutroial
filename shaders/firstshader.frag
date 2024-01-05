#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;

layout (location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUbo
{
    mat4 projectionMatrix;
    vec4 ambientColor; 
    vec3 lightPosition;
    vec4 lightColor; 
} ubo;

layout(push_constant) uniform PushConstants
{
    mat4 modelMatrix;
    mat4 noramlMaxtrix;
} pushConstants;

void main()
{
    vec3 directionToLight = ubo.lightPosition - fragPosWorld; // 光源到顶点的向量
    float attenuation = 1.0 / dot(directionToLight, directionToLight);  // 衰减系数

    vec3 lightColor = ubo.lightColor.rgb * ubo.lightColor.a * attenuation;        // 光源颜色
    vec3 ambientLight = ubo.ambientColor.rgb * ubo.ambientColor.a;  // 环境光
    vec3 diffuseLight = lightColor * max(dot(normalize(fragNormalWorld), normalize(directionToLight)), 0.0);     // 漫反射光

    outColor = vec4((diffuseLight + ambientLight) * fragColor, 1.0);
}