#version 450

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

layout(push_constant) uniform PushConstants
{
    mat2 transform;
    vec2 offset;
    vec3 color;
} pushConstants;

void main()
{
    gl_Position = vec4(pushConstants.transform * position + pushConstants.offset, 0.0, 1.0);
}