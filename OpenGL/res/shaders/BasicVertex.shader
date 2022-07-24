#version 330 core
layout(location = 0) in vec4 aPosition;
//layout(location = 1) in vec3 aColor;

out vec3 ourColor;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * aPosition;
   ourColor = aPosition.xyz;
};