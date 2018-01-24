#version 330 core

uniform mat4 MVP;

in vec2 vPos;
in vec4 vCol;
in vec2 vTex;

out vec4 color;
out vec2 texCoord;

void main() {
    gl_Position = MVP * vec4(vPos, 0.0, 1.0);
    color = vCol;
    texCoord = vTex;
}