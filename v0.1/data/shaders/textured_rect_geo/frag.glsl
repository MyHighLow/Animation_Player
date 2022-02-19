#version 460

uniform sampler2D u_texture;

in vec2 g_texCoord;

out vec4 FragColor;

void main() {

	FragColor = texture(u_texture, g_texCoord);
	//FragColor = vec4(g_texCoord.xy, 0.5f, 1.0f);

};
