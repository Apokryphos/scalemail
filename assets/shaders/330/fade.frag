#version 330 core

out vec4 fragColor;

uniform float fadeProgress;
uniform vec3 fadeColor;

void main() {
    fragColor = vec4(fadeColor.rgb, fadeProgress);
}