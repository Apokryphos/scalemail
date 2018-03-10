#version 120

uniform sampler2D texture1;
uniform float time;

varying vec2 texCoord;

void main() {
    vec4 texel = texture2D(texture1, vec2(texCoord.x + time, texCoord.y));

    if (texel.a == 0) {
        discard;
    }

    gl_FragColor = texel;
}