#version 330 core

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    //vec4 tempTexture = texture(aTexture, texcoords);
    //FragColor = vec4(backgroundColor.xyz * (1 - tempTexture.w) + tempTexture.xyz * tempTexture.w, 1.0);
    FragColor = vec4(lightColor * objectColor, 1.0);
}