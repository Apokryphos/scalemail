#version 120

uniform mat4 MVP;

attribute vec2 vPos;
attribute vec4 vCol;
attribute vec2 vTex;

varying vec4 color;
varying vec2 texCoord;

void main() {
    gl_Position = MVP * vec4(vPos, 0.0, 1.0);
    color = vCol;
    texCoord = vTex;
}