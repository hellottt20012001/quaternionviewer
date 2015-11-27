#include <graphics.h>
#include <object.h>

int Graphics::init()
{
	if(!glfwInit()) {std::cout << "Failed to initialize GLFW\n"; return 0;} // Initialise GLFW
    if(!initWindow()) {std::cout << "Failed to initialize GLFW Window\n"; return 0;} // Initialise GLFW Window
	glewExperimental = true;
	if(glewInit() != GLEW_OK) { std::cout << "Failed to initialize GLEW\n"; return 0;} // Initialize GLEW

	if(!data.readFromCSV("AS.csv")) return 0;

	program.load();
	cube.createCube();
	plate.createPlate();

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
		std::cout << "Package Number: "<< data[frame].Package_NO <<"     \xd";
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

	if(glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS && frame < data.size())
	{
		static float lastTime_KEY_A = glfwGetTime();
		float currentTime = glfwGetTime();
		if (currentTime - lastTime_KEY_A > frameSpeed) { frame ++; lastTime_KEY_A = currentTime; }
	}

	if(glfwGetKey(Window, GLFW_KEY_Z) == GLFW_PRESS && frame > 0)
	{
		static float lastTime_KEY_Z = glfwGetTime();
		float currentTime = glfwGetTime();
		if (currentTime - lastTime_KEY_Z > frameSpeed) { frame --; lastTime_KEY_Z = currentTime; }
	}
}

void Graphics::draw()
{
	// Clear the screen (dark blue background)
	glClearColor(0.0f, 0.0f, 0.05f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program.draw(cube, view.MVP());
	program.draw(plate, view.MVP());
}
