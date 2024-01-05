#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPosWorld;
layout(location = 2) out vec3 fragNormalWorld;

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
    vec4 positionWorldSpace = pushConstants.modelMatrix * vec4(position, 1.0);
    gl_Position = ubo.projectionMatrix * positionWorldSpace;

    // 将法线转换到世界空间
    fragNormalWorld = normalize(mat3(pushConstants.noramlMaxtrix) * normal);
    fragPosWorld = positionWorldSpace.xyz;
    
    fragColor = color;
}