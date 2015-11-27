#version 120

uniform vec4 orientation;
uniform mat4 MVP;
attribute vec3 VertexXYZ;
attribute vec3 VertexColor;
varying vec3 Color;

vec4 qconj(vec4 Q)
{
    return vec4(-Q.x, -Q.y, -Q.z, Q.w);
}

vec4 qprod(vec4 L, vec4 R)
{
    return vec4(
		L.w*R.x + L.x*R.w + L.y*R.z - L.z*R.y,
		L.w*R.y + L.y*R.w + L.z*R.x - L.x*R.z,
		L.w*R.z + L.z*R.w + L.x*R.y - L.y*R.x,
		L.w*R.w - L.x*R.x - L.y*R.y - L.z*R.z);
}

void main()
{
	//gl_Position = MVP * vec4(VertexXYZ, 1);
	gl_Position = MVP * vec4(qprod(qprod(orientation, vec4(VertexXYZ, 0)), qconj(orientation)).xyz, 1);
    Color = VertexColor;
}
