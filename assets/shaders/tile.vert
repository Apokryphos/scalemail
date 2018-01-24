#version 330 core

uniform mat4 MVP;

in vec2 vPos;
in vec2 vTex;

out vec2 texCoord;

void main() {
    gl_Position = MVP * vec4(vPos, 0.0, 1.0);
    texCoord = vTex;
}