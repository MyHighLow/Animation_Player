#version 460

uniform sampler2D u_texture;

in vec2 g_texCoord;
in vec3 g_color;

out vec4 FragColor;

void main() {

	FragColor = texture(u_texture, g_texCoord) * vec4(g_color.xyz, 1.0f);
	if (FragColor.w < 0.5f) {
		discard;
	};

};
