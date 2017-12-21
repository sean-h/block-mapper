#version 330 core
in vec3 color;
in vec2 texCoord;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
   FragColor = texture(ourTexture, texCoord) * vec4(color, 1.0);
}