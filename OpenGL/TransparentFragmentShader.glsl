#version 330 core

uniform vec3 objectColor;
uniform float opacity;

out vec4 FragColor;

void main()
{
   FragColor = vec4(objectColor, opacity);
}