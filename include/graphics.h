#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include "gl_2_1.h"

#include <thread>

#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <shader.h>
#include <view.h>
#include <object.h>
#include <quatview.h>
#include <text.h>
#include <tcp.h>

class Graphics
{
    private:
        int width = 1024, height = 720;
        GLFWwindow* Window;

        QuatViewProgram program;
		QuatViewObject cube;
		QuatViewObject plate;
        View view;
		FreeTypeEngine freeTypeEngine;

		int frame = 0;
		int time = 0;

    public:

		TCPserver tcpServer;

		float frameSpeed = 8;
		Data data;

        int init();
        int initWindow();
        void loop();
		void inputs();
        void draw();

        Graphics() : tcpServer(1234) {}
        ~Graphics() { glfwTerminate(); }

		void operator()()
		{
			if(init())
			{
				std::thread mthread2(std::ref(tcpServer));
				loop();
				mthread2.join();
			}
		}
};

#endif // GRAPHICS_H_INCLUDED
