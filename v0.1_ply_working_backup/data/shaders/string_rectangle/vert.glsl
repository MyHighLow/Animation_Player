#version 460

layout (location=0) in vec3 a_position;
layout (location=1) in vec2 a_dimensions;
layout (location=2) in vec3 a_color;

out vec2 v_dimensions;
out vec3 v_color;

void main() {

	v_dimensions = a_dimensions;
	v_color = a_color;
	gl_Position = vec4(a_position, 1.0f);

};
