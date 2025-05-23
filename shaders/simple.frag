#version 450

layout(location = 0) in vec3 inColor;

layout(push_constant) uniform Push {
    vec2 offset;
    vec3 color;
} push;

layout (location = 0) out vec4 outColor;

void main() {
    outColor = vec4(inColor * push.color, 1.0);
}