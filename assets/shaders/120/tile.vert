#version 120

uniform mat4 MVP;

attribute vec3 vPos;
attribute vec2 vTex;

varying vec2 texCoord;

void main() {
    gl_Position = MVP * vec4(vPos, 1.0);
    texCoord = vTex;
}