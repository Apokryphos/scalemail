#version 330 core

in vec4 color;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D texture1;
uniform float alpha;

void main() {
    vec4 texel = texture(texture1, texCoord);

    if (texel.a == 0) {
        discard;
    }

    fragColor = texel * color * alpha;
}