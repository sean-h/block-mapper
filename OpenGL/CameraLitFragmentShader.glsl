#version 330 core

in vec3 Normal;
in vec3 FragPosition;

uniform vec3 objectColor;
uniform vec3 cameraPosition;

out vec4 FragColor;

void main()
{
    float distance = length(cameraPosition - FragPosition);

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(cameraPosition - FragPosition);
    float lightDot = dot(norm, lightDir);
    float diff = max(lightDot, 0.0);
    vec3 diffuse = vec3(1.0, 1.0, 1.0) * (diff * objectColor);

    FragColor = vec4(diffuse, 1.0);
}