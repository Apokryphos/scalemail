#version 120

uniform float fadeProgress;
uniform vec3 fadeColor;

void main() {
    gl_FragColor = vec4(fadeColor.rgb, fadeProgress);
}