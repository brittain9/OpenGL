#version 330 core
layout(location = 0) out vec4 color;

in vec3 ourColor;

uniform vec4 u_Color;

void main()
{
	color = vec4(ourColor, 1.0) * u_Color;
};