#version 330 core
in vec3 col;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D aTexture;
uniform vec3 backgroundColor;

void main()
{
    vec4 tempTexture = texture(aTexture, TexCoord);
    //FragColor = tempTexture;
    //FragColor = vec4(tempTexture.xyz + backgroundColor * (1 - tempTexture.w) , 1.0);
    FragColor = vec4(1.0,1.0,1.0,1.0);
}