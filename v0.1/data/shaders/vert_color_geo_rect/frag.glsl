#version 460

in vec3 g_color;

out vec4 FragColor;

void main() {

	FragColor = vec4(g_color, 1.0f);
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);

};
