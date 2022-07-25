#version 330 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 ourColor;

uniform vec4 u_Color;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

void main()
{
	vec4 texColor = texture(u_Texture1, v_TexCoord) * vec4(ourColor, 1.0);
	color = texColor;
};