#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

// GLFW
#include <GLFW/glfw3.h>

#include <quaternion.h>
#include <vec.h>

class View
{
public:
	float FoV = M_PI/180.0 * 45.0f;
	float aspect = 1024.0f/720.0f;
	float near = 0.1f, far = 100.0f;

	quat cameraVector;
	quat upVector;
    float rotationSpeed = 2E-4f;

	mat4 MVP();

    //float FoV_min = M_PI/180.0 * 5.0f, FoV_max = M_PI/180.0 * 135.0f;
    //float ZoomSpeed = M_PI/180.0 * 50.0f;

	void rotateFromMouseInput(int x, int y);
	void setTopView() { cameraVector = vec3(0,0,5); upVector = quat(0,1,0); }
	void setFrontView() { cameraVector = vec3(0,5,0); upVector = quat(0,0,1); }
	void setBackView() { cameraVector = vec3(0,-5,0); upVector = quat(0,0,1); }
	void setLeftView() { cameraVector = vec3(-5,0,0); upVector = quat(0,0,1); }
	void setRightView() { cameraVector = vec3(5,0,0); upVector = quat(0,0,1); }

	View() { setFrontView(); }
};

#endif // VIEW_H_INCLUDED
