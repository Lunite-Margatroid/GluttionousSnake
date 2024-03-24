#version 330 core
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform float u_Width;

in vec4 Color[];
out vec4 GColor;

void main()
{
	gl_Position = gl_in[0].gl_Position;
	GColor = Color[0];
	EmitVertex();
	
	gl_Position = gl_in[0].gl_Position + vec4(u_Width, 0.0f, 0.0f, 0.0f);
	GColor = Color[0];
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(0.0f,u_Width, 0.0f, 0.0f);
	
	GColor = Color[0];
	EmitVertex();
	
	gl_Position = gl_in[0].gl_Position + vec4(u_Width, u_Width, 0.0f, 0.0f);
	GColor = Color[0];
	EmitVertex();
	
	EndPrimitive();
	
	
}