#version 330 core

uniform mat4 MVP;

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec4 vCol;
layout (location = 2) in vec2 vTex;

out vec4 color;
out vec2 texCoord;

void main() {
    gl_Position = MVP * vec4(vPos, 1.0);
    color = vCol;
    texCoord = vTex;
}