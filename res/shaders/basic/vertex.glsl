#version 330 core
in vec2 localPos;
layout (std140) uniform Screen
{
    mat4 projection;
    float width;
    float height;
};
void main(){
    gl_Position = projection*vec4(localPos, 1.0f, 1.0f);
}