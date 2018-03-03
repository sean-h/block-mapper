#version 330 core

in vec3 Normal;
in vec3 FragPosition;
in vec2 TexCoords;

uniform vec3 objectColor;
uniform vec3 cameraPosition;
uniform sampler2D tex;
uniform float inverseColorMultiplier;

out vec4 FragColor;

void main()
{
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(cameraPosition - FragPosition);
    float lightDot = dot(norm, lightDir);
    float diff = max(lightDot, 0.0);
    vec3 diffuse = vec3(1.0, 1.0, 1.0) * (diff * objectColor * texture(tex, TexCoords).xyz);

    vec3 inverseColor = vec3(1.0 - diffuse.x, 1.0 - diffuse.y, 1.0 - diffuse.z);
    vec3 color = (diffuse * (1.0 - inverseColorMultiplier)) + (inverseColor * inverseColorMultiplier);

    FragColor = vec4(color, 1.0);
}