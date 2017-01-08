#ifdef WEBGL
precision mediump float;
#define out varying
#define in
#define layout(args) attribute
#endif

uniform mat4 transformMatrix;
uniform mat4 cameraMatrix;
uniform mat4 projectionMatrix;
uniform vec4 rotor;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textCoord;
layout(location = 2) in vec3 normal;

out vec3 vertexCoord;
out vec2 textureCoord;
out vec3 normalVec;

vec4 qprod(vec4 q1, vec4 q2)
{
	return vec4 (
    	q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x,
    	-q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y,
    	q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z,
    	-q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w
		);
}

vec4 qconj(vec4 q)
{
	return vec4 (-q.x, -q.y, -q.z, q.w);
}

void main()
{
	vec4 r = rotor;
	vec4 pos = qprod(qconj(r), qprod(vec4(position, 0.), r));
	vec4 post = transformMatrix * vec4(pos.xyz, 1.);
	vec4 nor = qprod(qconj(r), qprod(vec4(normal, 0.), r));
	vec4 nort = transformMatrix * vec4(nor.xyz, 1.);
	gl_Position 	= projectionMatrix * cameraMatrix * post;
	textureCoord 	= textCoord;
	normalVec 		= nort.xyz;
	vertexCoord 	= post.xyz;
}
