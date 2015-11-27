#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

// GLEW
#include <GL/glew.h>

// GLFW
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <shader.h>
#include <view.h>
#include <object.h>
#include <quatview.h>

class Graphics
{
        int width = 1024, height = 768;
        GLFWwindow* Window;

        QuatViewProgram program;
		QuatViewObject cube;
		QuatViewObject plate;
        View view;
		Data data;

		int frame = 0;

    public:

		float frameSpeed = 0.1;

        int init();
        int initWindow();
        void loop();
		void inputs();
        void draw();

        Graphics() {}
        ~Graphics() { glfwTerminate(); }
};

#endif // GRAPHICS_H_INCLUDED
