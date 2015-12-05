#version 120

//uniform sampler2D Sampler;
varying vec3 Color;

void main()
{
	gl_FragColor = vec4(Color, 1.0);
}
