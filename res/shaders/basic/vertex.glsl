#version 330 core
in vec2 localPos;
layout (std140) uniform Camera {
    mat4 projection;
    mat4 view;
};
void main(){
    gl_Position = projection*view*vec4(localPos, 1.0f, 1.0f);
}