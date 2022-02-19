#version 460

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec2 v_dimensions[];
in vec3 v_color[];

out vec3 g_color;


void main() {

	g_color = v_color[0];

	vec4 p2 = gl_in[0].gl_Position + vec4(v_dimensions[0].xy, 0.0f, 0.0f);

	//gl_Position = gl_in[0].gl_Position.xyzw;
	gl_Position = vec4(0.5f, 0.5f, 0.0f, 0.0f);
	EmitVertex();
	
	//gl_Position = vec4(gl_in[0].gl_Position.x, p2.yzw);
	gl_Position = vec4(-0.5f, 0.5f, 0.0f, 0.0f);
	EmitVertex();
	
	//gl_Position = vec4(p2.x, gl_in[0].gl_Position.yzw);
	gl_Position = vec4(0.5f, -0.5f, 0.0f, 0.0f);
	EmitVertex();
	
	//gl_Position = p2.xyzw;
	gl_Position = vec4(0.5f, 0.5f, 0.0f, 0.0f);
	EmitVertex();
	
	EndPrimitive();
};
