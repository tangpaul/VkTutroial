#version 450

layout (location = 0) out vec4 outColor;

layout(push_constant) uniform PushConstants
{
    mat2 transform;
    vec2 offset;
    vec3 color;
} pushConstants;

void main()
{
    outColor = vec4(pushConstants.color, 1.0);
}