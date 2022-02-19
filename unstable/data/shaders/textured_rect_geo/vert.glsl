#version 460

layout (location=0) in vec3 a_position;
layout (location=1) in vec2 a_dimensions;
layout (location=2) in vec2 a_texPosition;
layout (location=3) in vec2 a_texDimensions;

out vec2 v_dimensions;
out vec3 v_color;
out vec4 v_packedTex;

void main() {

	v_dimensions = a_dimensions;
	v_packedTex = vec4(a_texPosition.xy, a_texDimensions.xy);
	gl_Position = vec4(a_position, 1.0f);

};
