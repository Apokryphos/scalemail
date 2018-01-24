#version 330 core

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D texture1;
uniform float time;

void main() {
    vec4 texel = texture(texture1, vec2(texCoord.x + time, texCoord.y));
    fragColor = texel;
}