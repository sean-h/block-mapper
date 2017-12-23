#version 330 core

in vec3 Normal;
in vec3 FragPosition;
in vec3 LightPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPosition;

out vec4 FragColor;

void main()
{
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPosition);
    float lightDot = dot(norm, lightDir);
    float diff = max(lightDot, 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    vec3 specular = vec3(0.0, 0.0, 0.0);
    if (lightDot > 0)
    {
        float specularStrength = 0.5;
        vec3 viewDir = normalize(-FragPosition);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        specular = specularStrength * spec * lightColor;
    }

    vec3 color = (ambient + diffuse + specular) * objectColor;
    FragColor =  vec4(color, 1.0);
}