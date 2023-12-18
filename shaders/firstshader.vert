#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;

layout(set = 0, binding = 0) uniform GlobalUbo
{
    mat4 projectionMatrix;
    vec3 directionToLight;
} ubo;

layout(push_constant) uniform PushConstants
{
    mat4 modelMatrix;
    mat4 noramlMaxtrix;
} pushConstants;

const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0, -3.0, -1.0));
const float AMBIENT = 0.02;

void main()
{
    gl_Position = ubo.projectionMatrix * pushConstants.modelMatrix * vec4(position, 1.0);

    // 将法线转换到世界空间
    // vec3 normalWorldSpace = normalize(mat3(pushConstants.modelMatrix) * normal);
    vec3 normalWorldSpace = normalize(mat3(pushConstants.noramlMaxtrix) * normal);
    // 计算光照强度 通过点乘法计算光照强度 (越垂直越亮)
    float lightIndensity = AMBIENT + max(dot(normalWorldSpace, DIRECTION_TO_LIGHT), 0.0);

    fragColor = lightIndensity * color;
}