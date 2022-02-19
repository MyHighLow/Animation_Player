#version 460

in vec2 v_tex_coord;
uniform sampler2D u_texture;

out vec4 FragColor;

void main() {
	FragColor = vec4(texture(u_texture, v_tex_coord).rrr, 1.0f);
	//FragColor = vec4(v_tex_coord, 0.0f, 1.0f);
};
