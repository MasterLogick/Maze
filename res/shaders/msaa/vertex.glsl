#version 330 core
in vec2 position;
in vec2 texCord;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    TexCoords = texCord;
}