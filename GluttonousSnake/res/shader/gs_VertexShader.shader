#version 330 core
layout(location = 0) in vec2 aCoord;
layout(location = 1) in vec4 aColor;

uniform mat4 u_ProjectionTrans;
uniform mat4 u_ViewTrans;

out vec4 Color;

void main()
{
	gl_Position = u_ProjectionTrans * u_ViewTrans * vec4(aCoord, 0.0f, 1.0f);
	Color = aColor;
}
