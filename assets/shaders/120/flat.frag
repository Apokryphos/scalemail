#version 120

uniform sampler2D texture1;

varying vec4 color;
varying vec2 texCoord;

void main() {
    vec4 texel = texture2D(texture1, texCoord);
    gl_FragColor = texel * color;
}