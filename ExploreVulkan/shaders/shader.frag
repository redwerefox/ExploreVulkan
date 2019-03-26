
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;
layout(location = 0) in vec3 color;

vec4 red = vec4(1.0f, 0.0f, 0.0f, 1.0f);

void main () {
	outColor = vec4(color, 1.0f);
}