#version 460

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec2 v_dimensions[];
in vec3 v_color[];

out vec2 g_texCoord;
out vec3 g_color;


void main() {

	g_color = v_color[0];

	vec4 p2 = gl_in[0].gl_Position + vec4(v_dimensions[0].xy, 0.0f, 0.0f);

	gl_Position = gl_in[0].gl_Position.xyzw;
	g_texCoord = vec2(0.0f,1.0f);
	EmitVertex();
	
	gl_Position = vec4(gl_in[0].gl_Position.x, p2.yzw);
	g_texCoord = vec2(0.0f, 0.0f);
	EmitVertex();
	
	gl_Position = vec4(p2.x, gl_in[0].gl_Position.yzw);
	g_texCoord = vec2(1.0f, 1.0f);
	EmitVertex();
	
	gl_Position = p2.xyzw;
	g_texCoord = vec2(1.0f,0.0f);
	EmitVertex();
	
	EndPrimitive();
};
