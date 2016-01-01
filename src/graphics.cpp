#include <graphics.h>

int Graphics::init()
{
	if(!glfwInit()) {std::cout << "Failed to initialize GLFW\n"; return 0;} // Initialise GLFW
	std::cout << "GLFW initialized\n";
    if(!initWindow()) {std::cout << "Failed to initialize GLFW Window\n"; return 0;} // Initialise GLFW Window
	std::cout << "GLFW window created\n";
	if(ogl_LoadFunctions() == ogl_LOAD_FAILED) { std::cout << "Failed to load OpenGL functions\n"; return 0;}
	std::cout << "OpenGL functions loaded\n";

	// Get version info
	const GLubyte* Renderer = glGetString(GL_RENDERER);
	const GLubyte* Version  = glGetString(GL_VERSION);
	printf("Renderer: %s\n", Renderer);
	printf("OpenGL version %s\n\n", Version);

	time = data[0].Time;

	program.load(); std::cout << "Program loaded\n";
	cube.createCube(); std::cout << "Cube created\n";
	plate.createPlate(); std::cout << "Plate created\n";

	freeTypeEngine.initFreeType();
	freeTypeEngine.load();
	std::cout<< "Freetype engine initialized\n";

	return 1;
}

int Graphics::initWindow()
{
    // Open a GLFW window and create its OpenGL context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2); glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	//glfwWindowHint(GLFW_SAMPLES, 4);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	Window = glfwCreateWindow(width, height, "Quaternion Viewer", NULL, NULL);
	if( Window == NULL ){ std::cout << "Failed to initialize GLFW Window\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(Window);

    // Ensure we can capture the escape key being pressed
	glfwSetInputMode(Window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(Window, width/2, height/2);

	return 1;
}

void Graphics::loop()
{
	do
    {
        inputs();

		/*std::cout << "Time: ";
		int hr = time / 3600, min = (time % 3600) / 60, sec = time % 60;
		std::cout << hr << ":";
		if(min < 10) std::cout << "0";
		std::cout << min << ":";
		if(sec < 10) std::cout << "0";
		std::cout << sec;
		std::cout << "\t";

		std::cout << "Package: ";
		std::cout << data[frame].Package_NO;
		std::cout << "\t";

		std::cout << "     \xd";*/
		cube.orientation = tcpServer.orientation;
		draw();
        glfwSwapBuffers(Window); glfwPollEvents();
    }
    while(glfwGetKey(Window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(Window) == 0);
}

void Graphics::inputs()
{
	if(glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		// Cursor position
    	double u, v; glfwGetCursorPos(Window, &u, &v);
		view.rotateFromMouseInput(u - width/2, v - height/2);
	}

	if(glfwGetKey(Window, GLFW_KEY_ENTER) == GLFW_PRESS)
		view.setTopView();
	else if(glfwGetKey(Window, GLFW_KEY_DOWN) == GLFW_PRESS)
		view.setFrontView();
	else if(glfwGetKey(Window, GLFW_KEY_UP) == GLFW_PRESS)
		view.setBackView();
	else if(glfwGetKey(Window, GLFW_KEY_LEFT) == GLFW_PRESS)
		view.setLeftView();
	else if(glfwGetKey(Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		view.setRightView();

	if(glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS && time < data[data.size()-1].Time)
	{
		static float lastTime_KEY_A = glfwGetTime();
		float currentTime = glfwGetTime();
		if (currentTime - lastTime_KEY_A > 1.0f/frameSpeed) { time++; lastTime_KEY_A = currentTime; while(data[frame+1].Time <= time) frame++; }
	}

	if(glfwGetKey(Window, GLFW_KEY_Z) == GLFW_PRESS && time > data[0].Time)
	{
		static float lastTime_KEY_Z = glfwGetTime();
		float currentTime = glfwGetTime();
		if (currentTime - lastTime_KEY_Z > 1.0f/frameSpeed) { time--; lastTime_KEY_Z = currentTime; while(data[frame-1].Time >= time) {frame--; if(frame < 1) break;} }
	}

	if(glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS && frameSpeed < 64)
	{
		static float lastTime_KEY_S = glfwGetTime();
		float currentTime = glfwGetTime();
		if (currentTime - lastTime_KEY_S > 0.2) { frameSpeed *= 2; lastTime_KEY_S = currentTime; }
	}

	if(glfwGetKey(Window, GLFW_KEY_X) == GLFW_PRESS && frameSpeed > 0.125)
	{
		static float lastTime_KEY_X = glfwGetTime();
		float currentTime = glfwGetTime();
		if (currentTime - lastTime_KEY_X > 0.2) { frameSpeed /= 2; lastTime_KEY_X= currentTime; }
	}

	if(glfwGetKey(Window, GLFW_KEY_HOME) == GLFW_PRESS)
	{
		frame = 0;
		time = data[frame].Time;
	}

	if(glfwGetKey(Window, GLFW_KEY_END) == GLFW_PRESS)
	{
		frame = data.size()-1;
		time = data[frame].Time;
	}

	if(glfwGetKey(Window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
	{
		static float lastTime_KEY_PAGEDOWN = glfwGetTime();
		float currentTime = glfwGetTime();
		if (currentTime - lastTime_KEY_PAGEDOWN > 0.2)
		{
			if (frame + 100 < data.size()) frame += 100;
			else frame = data.size()-1;
			time = data[frame].Time;
			lastTime_KEY_PAGEDOWN= currentTime;
		}
	}

	if(glfwGetKey(Window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
	{
		static float lastTime_KEY_PAGEUP = glfwGetTime();
		float currentTime = glfwGetTime();
		if (currentTime - lastTime_KEY_PAGEUP > 0.2)
		{
			if (frame - 100 > 0) frame -= 100;
			else frame = 0;
			time = data[frame].Time;
			lastTime_KEY_PAGEUP= currentTime;
		}
	}
}

void Graphics::draw()
{
	// Clear the screen (dark blue background)
	glClearColor(0, 0, 0.1, 1);
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program.draw(cube, view.MVP());
	program.draw(plate, view.MVP());

	float sx = 2.0 / width;
 	float sy = 2.0 / height;
	std::stringstream renderStream;

	freeTypeEngine.render("Press HOME to go to start of data.", -1 + 680 * sx, 1 - 30 * sy, sx, sy);
	freeTypeEngine.render("Press END to go to end of data.", -1 + 680 * sx, 1 - 50 * sy, sx, sy);
	freeTypeEngine.render("Press PAGE UP to go back 100 frames.", -1 + 680 * sx, 1 - 70 * sy, sx, sy);
	freeTypeEngine.render("Press PAGE DOWN to go forward 100 frames.", -1 + 680 * sx, 1 - 90 * sy, sx, sy);

	freeTypeEngine.render("Press A/Z to play forward/backward.", -1 + 720 * sx, 1 - 130 * sy, sx, sy);
	freeTypeEngine.render("Press S/X to adjust playback speed.", -1 + 720 * sx, 1 - 150 * sy, sx, sy);
	freeTypeEngine.render("Hold RIGHT mouse button to rotate view.", -1 + 720 * sx, 1 - 170 * sy, sx, sy);

	freeTypeEngine.render("Press DOWN = FRONT view.", -1 + 800 * sx, 1 - 210 * sy, sx, sy);
	freeTypeEngine.render("Press UP = BACK view.", -1 + 800 * sx, 1 - 230 * sy, sx, sy);
	freeTypeEngine.render("Press LEFT = LEFT view.", -1 + 800 * sx, 1 - 250 * sy, sx, sy);
	freeTypeEngine.render("Press RIGHT = RIGHT view.", -1 + 800 * sx, 1 - 270 * sy, sx, sy);
	freeTypeEngine.render("Press ENTER = TOP view.", -1 + 800 * sx, 1 - 290 * sy, sx, sy);

	freeTypeEngine.render("TOP = WHITE", -1 + 860 * sx, 1 - 330 * sy, sx, sy);
	freeTypeEngine.render("BOTTOM = YELLOW", -1 + 860 * sx, 1 - 350 * sy, sx, sy);
	freeTypeEngine.render("RIGHT = RED", -1 + 860 * sx, 1 - 370 * sy, sx, sy);
	freeTypeEngine.render("LEFT = ORANGE", -1 + 860 * sx, 1 - 390 * sy, sx, sy);
	freeTypeEngine.render("FRONT = GREEN", -1 + 860 * sx, 1 - 410 * sy, sx, sy);
	freeTypeEngine.render("BACK = BLUE", -1 + 860 * sx, 1 - 430 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << "Time: ";
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 10 * sx, 1 - 30 * sy, sx, sy);

	renderStream = std::stringstream();
	int hr = time / 3600, min = (time % 3600) / 60, sec = time % 60;
	renderStream << hr << ":";
	if(min < 10) renderStream << "0";
	renderStream << min << ":";
	if(sec < 10) renderStream << "0";
	renderStream << sec;
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 180 * sx, 1 - 30 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << "Package: ";
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 10 * sx, 1 - 50 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << data[frame].Package_NO;
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 180 * sx, 1 - 50 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << "Orientation: ";
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 10 * sx, 1 - 90 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << data[frame].Orientation;
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 180 * sx, 1 - 90 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << "Temperature: ";
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 10 * sx, 1 - 110 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << data[frame].Temperature;
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 180 * sx, 1 - 110 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << "Outside Temperature: ";
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 10 * sx, 1 - 130 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << data[frame].OutsideTemperature;
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 180 * sx, 1 - 130 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << "Pressure: ";
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 10 * sx, 1 - 150 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << data[frame].Pressure;
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 180 * sx, 1 - 150 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << "Voltage: ";
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 10 * sx, 1 - 170 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << data[frame].Voltage;
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 180 * sx, 1 - 170 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << "RSSI: ";
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 10 * sx, 1 - 190 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << data[frame].RSSI;
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 180 * sx, 1 - 190 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << "Frequency Error: ";
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 10 * sx, 1 - 210 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << data[frame].Frequency;
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 180 * sx, 1 - 210 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << "CRC: ";
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 10 * sx, 1 - 230 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << data[frame].CRC;
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 180 * sx, 1 - 230 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << "Playback speed: ";
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 10 * sx, 1 - 270 * sy, sx, sy);

	renderStream = std::stringstream();
	renderStream << frameSpeed;
	renderStream << "x";
	freeTypeEngine.render(renderStream.str().c_str(), -1 + 180 * sx, 1 - 270 * sy, sx, sy);
}
