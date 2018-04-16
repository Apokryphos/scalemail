#version 120

uniform mat4 ProjMtx;
in vec2 Position;
in vec2 UV;
in vec4 Color;
varying out vec2 Frag_UV;
varying out vec4 Frag_Color;

void main()
{
	Frag_UV = UV;
	Frag_Color = Color;
	gl_Position = ProjMtx * vec4(Position.xy,0,1);
};