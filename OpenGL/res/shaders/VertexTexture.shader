#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;
out vec3 ourColor;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * position;
   ourColor = aColor;
   v_TexCoord = texCoord;
};