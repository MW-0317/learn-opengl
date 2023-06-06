#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 texcoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    texcoords = vec2(aTexCoords.x, aTexCoords.y);
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}