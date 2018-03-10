#version 120

uniform mat4 MVP;

attribute in vec3 vPos;
attribute in vec2 vTex;
attribute in vec4 vCol;

varying vec4 color;
varying vec2 texCoord;

void main() {
    gl_Position = MVP * vec4(vPos, 1.0);
    color = vCol;
    texCoord = vTex;
}