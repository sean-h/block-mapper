#version 330 core

in vec3 FragPosition;

uniform vec3 objectColor;

out vec4 FragColor;

void main()
{
    float lineWidth = 0.05;
    vec3 offsetPosition = FragPosition + vec3(0.5, 0.0, 0.5);
    float fractX = fract(offsetPosition.x);
    float fractZ = fract(offsetPosition.z);
    if (fractX > lineWidth && fractX < 1.0 - lineWidth && fractZ > lineWidth && fractZ < 1.0 - lineWidth)
    {
        discard;
    }

    FragColor = vec4(objectColor, 1.0);
}