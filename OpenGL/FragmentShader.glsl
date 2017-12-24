#version 330 core

in vec3 Normal;
in vec3 FragPosition;
in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
}; 

uniform Material material;

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

uniform vec3 viewPosition;

out vec4 FragColor;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPosition);
    float lightDot = dot(norm, lightDir);
    float diff = max(lightDot, 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));

    // Specular
    vec3 specular = vec3(0.0, 0.0, 0.0);
    if (lightDot > 0)
    {
        vec3 viewDir = normalize(light.position - FragPosition);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    }

    vec3 color = ambient + diffuse + specular;
    FragColor =  vec4(color, 1.0);
}