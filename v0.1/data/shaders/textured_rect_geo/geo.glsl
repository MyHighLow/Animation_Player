#version 460

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec2 v_dimensions[];
in vec3 v_color[];
in vec4 v_packedTex[];

out vec2 g_texCoord;


void main() {

	vec4 p2 = gl_in[0].gl_Position + vec4(v_dimensions[0].xy, 0.0f, 0.0f);
	vec2 t2 = v_packedTex[0].xy + v_packedTex[0].zw;

	gl_Position = gl_in[0].gl_Position.xyzw;
	g_texCoord = vec2(v_packedTex[0].xy);
	EmitVertex();
	
	gl_Position = vec4(gl_in[0].gl_Position.x, p2.yzw);
	g_texCoord = vec2(v_packedTex[0].x, t2.y);
	EmitVertex();
	
	gl_Position = vec4(p2.x, gl_in[0].gl_Position.yzw);
	g_texCoord = vec2(t2.x, v_packedTex[0].y);
	EmitVertex();
	
	gl_Position = p2.xyzw;
	g_texCoord = t2;
	EmitVertex();
	
	EndPrimitive();
};
