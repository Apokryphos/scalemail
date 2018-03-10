#version 120

uniform sampler2D texture1;
uniform float alpha;

varying vec4 color;
varying vec2 texCoord;

void main() {
    vec4 texel = texture2D(texture1, texCoord);

    if (texel.a == 0) {
        discard;
    }

    gl_FragColor = texel * color * alpha;
}