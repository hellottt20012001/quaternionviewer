#include <view.h>

mat4 View::MVP()
{
	// Projection matrix : Field of View, aspect ratio, display range : near <-> far
	mat4 Projection = glm::perspective(FoV, aspect, near, far);

	// Camera matrix
	mat4 View = glm::lookAt(vec3(cameraVector), vec3(0,0,0), vec3(upVector));

	// Our ModelViewProjection : multiplication of our 3 matrices
	return Projection * View;
}

void View::rotateFromMouseInput(int x, int y)
{
	quat rightVector = normalize(cameraVector % upVector);
	quat rotationVector = (x * rightVector + y * upVector) * rotationSpeed * FoV;
	quat rotationAxis = rotationVector % cameraVector;
	quat R = rotor(rotationVector.norm(), rotationAxis);

	cameraVector =  R * cameraVector * ~R;
	upVector = R * upVector * ~R;
}
