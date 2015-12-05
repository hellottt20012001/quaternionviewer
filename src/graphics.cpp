#include <graphics.h>
#include <object.h>
#include <text.h>

int Graphics::init()
{
	if(!glfwInit()) {std::cout << "Failed to initialize GLFW\n"; return 0;} // Initialise GLFW
    if(!initWindow()) {std::cout << "Failed to initialize GLFW Window\n"; return 0;} // Initialise GLFW Window
	glewExperimental = true;
	if(glewInit() != GLEW_OK) { std::cout << "Failed to initialize GLEW\n"; return 0;} // Initialize GLEW

	if(!data.readFromCSV("AS.csv")) return 0;
	time = data[0].Time;

	program.load();
	cube.createCube();
	plate.createPlate();
	//freeTypeEngine.initFreeType();
	//freeTypeEngine.load();

	return 1;
}

int Graphics::initWindow()
{
    // Open a GLFW window and create its OpenGL context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	Window = glfwCreateWindow(width, height, "Quaternion Viewer", NULL, NULL);
	if( Window == NULL ){ std::cout << "Failed to initialize GLFW Window\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(Window);

    // Ensure we can capture the escape key being pressed
	glfwSetInputMode(Window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(Window, width/2, height/2);

	// Get version info
	const GLubyte* Renderer = glGetString (GL_RENDERER);
    const GLubyte* Version  = glGetString (GL_VERSION);
    printf("Renderer: %s\n", Renderer);
    printf("OpenGL version %s\n\n", Version);

	return 1;
}

void Graphics::loop()
{
	do
    {
        inputs();

		std::cout << "Time: ";
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

		/*std::cout << "Orientation: ";
		std::cout << data[frame].Orientation;
		std::cout << "\t";*/

		std::cout << "Temp: ";
		std::cout << data[frame].Temperature;
		std::cout << "\t";

		std::cout << "Pressure: ";
		std::cout << data[frame].Pressure;
		std::cout << "\t";

		/*std::cout << "Voltage: ";
		std::cout << data[frame].Voltage;
		std::cout << "\t";*/

		std::cout << "RSSI: ";
		std::cout << data[frame].RSSI;
		std::cout << "\t";

		std::cout << "Playback speed: ";
		std::cout << frameSpeed;
		std::cout << " x";
		std::cout << "\t";

		/*std::cout << "Freq Err: ";
		std::cout << data[frame].Frequency;
		std::cout << "\t";*/

		/*std::cout << "CRC: ";
		std::cout << data[frame].CRC;
		std::cout << "\t";*/

		std::cout << "     \xd";
		cube.orientation = data[frame].Orientation;
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

	if(glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
	{
		static float lastTime_KEY_S = glfwGetTime();
		float currentTime = glfwGetTime();
		if (currentTime - lastTime_KEY_S > 0.2) { frameSpeed *= 2; lastTime_KEY_S = currentTime; }
	}

	if(glfwGetKey(Window, GLFW_KEY_X) == GLFW_PRESS && frameSpeed > 0)
	{
		static float lastTime_KEY_X = glfwGetTime();
		float currentTime = glfwGetTime();
		if (currentTime - lastTime_KEY_X > 0.2) { frameSpeed /= 2; lastTime_KEY_X= currentTime; }
	}
}

void Graphics::draw()
{
	// Clear the screen (dark blue background)
	glClearColor(0, 0, 0.1, 1);
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program.draw(cube, view.MVP());
	program.draw(plate, view.MVP());

	/*float sx = 2.0 / width;
 	float sy = 2.0 / height;
	freeTypeEngine.render("The Quick Brown Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 50 * sy, sx, sy);*/
}
