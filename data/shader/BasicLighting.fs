#ifdef WEBGL
precision mediump float;
#define texture texture2D
#define out varying
#define in varying
#endif

struct PointLight
{
	vec3 position;
	vec3 radiant;
};

in vec3 vertexCoord;
in vec3 normalVec;
in vec2 textureCoord;

uniform sampler2D surfaceTexture;
uniform PointLight pointLight;
uniform vec3 ambient;

void main()
{
	vec3 diff = pointLight.position-vertexCoord;
	vec3 directionVec = normalize(diff);
	float dist = length(diff);
	vec3 lightStrengthOnFragment = pointLight.radiant * dot(normalVec, directionVec) / (dist*dist);
	if (lightStrengthOnFragment.x < 0.) lightStrengthOnFragment.x = 0.;
	if (lightStrengthOnFragment.y < 0.) lightStrengthOnFragment.y = 0.;
	if (lightStrengthOnFragment.z < 0.) lightStrengthOnFragment.z = 0.;
	vec4 textureColor = texture(surfaceTexture,textureCoord);

	if(dot(textureColor.xyz,vec3(1.)) == 0.)//If no texture is applied
		textureColor = vec4(1.,1.,1.,1.);

	// Apply lighting
	vec3 renderedColor = textureColor.xyz * (lightStrengthOnFragment + ambient);

	// Gamma correction
	const float gamma = 2.2;
	const float inverseGamma = 1./gamma;
	renderedColor = max(renderedColor,vec3(0.));
	vec3 correctedColor = pow(renderedColor,vec3(inverseGamma));

	gl_FragColor = vec4(correctedColor, 1.);
}
