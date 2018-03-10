#version 330 core

uniform mat4 MVP;

in vec3 vPos;
in vec4 vCol;

out vec4 color;

void main() {
    gl_Position = MVP * vec4(vPos, 1.0);
    color = vCol;
}