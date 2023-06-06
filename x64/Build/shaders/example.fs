#version 330 core

in vec2 texcoords;

out vec4 FragColor;

uniform sampler2D aTexture;
uniform vec3 backgroundColor;

void main()
{
    vec4 tempTexture = texture(aTexture, texcoords);
    FragColor = vec4(backgroundColor.xyz * (1 - tempTexture.w) + tempTexture.xyz * tempTexture.w, 1.0);
}