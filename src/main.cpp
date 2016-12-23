#include <Pulsar.hpp>
#include <PulsarMesh.hpp>
#include <PulsarShader.hpp>
#include <ThreeDShader.hpp>
#include <PulsarScene.hpp>
#include <PulsarWindow.hpp>
using namespace Pulsar;

#include <iostream>
#include <string>
using namespace std;

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

class MainWindow : public Window
{
public:
	MainWindow();
	~MainWindow();
	void render();
	void update();

protected:
	Renderer 		renderer;
	ThreeDShader* 	shader;
	SceneNode* 		rootNode;
	Camera* 		camera;
	Texture* 		texture;
	Mesh* 			cubeMesh;
	SceneMeshItem* 	cube;
	Mesh* 			plateMesh;
	SceneMeshItem* 	plate;

	void initShader();
	void initTexture();
	void initCamera();
	void initCube();
	void initPlate();
};

MainWindow::MainWindow()
{
	createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Attitude Visualizer");

	renderer.init();
	renderer.setClearColor(vec3(135, 206, 250)/255.f);

	initShader();
	initTexture();
	initCamera();

	rootNode = new SceneNode;
	initCube();
	initPlate();
}

void MainWindow::initShader()
{
	bool result = true;
	shader = new ThreeDShader;
	result &= shader->addVertexShader(File::readAllText("data/shader/BasicLighting.vs"));
	result &= shader->addFragmentShader(File::readAllText("data/shader/BasicLighting.fs"));
	result &= shader->compile();
	if(!result)
	{
		terminate();
		exit(0);
	}
	shader->Shader::bind();
	shader->setParameter("pointLight.position",vec3(0, -10, 10));
	shader->setParameter("pointLight.radiant",vec3(100.0f));
	shader->setParameter("ambient",vec3(30.f/255));
	shader->Shader::unbind();
}

void MainWindow::initTexture()
{
	vec4 colors[8] =
	{
		vec4(255, 213, 0, 255) / 255.f,
		vec4(255, 255, 255, 255) / 255.f,
		vec4(255, 88, 0, 255) / 255.f,
		vec4(196, 30, 58, 255) / 255.f,
		vec4(0, 158, 96, 255) / 255.f,
		vec4(0, 81, 186, 255) / 255.f,
		vec4(0, 255, 0, 255) / 255.f,
	};

	texture = new Texture;
	texture->fromRaw(colors, 8, 1);
	texture->enableMipmap(false);
	shader->setTexture(texture);
}

void MainWindow::initCamera()
{
	Projection projection;
	projection.setProjection(radians(70.0f), WINDOW_WIDTH, WINDOW_HEIGHT, 0.01f, 50.0f);

	camera = new Camera();
	camera->lookAt(vec3(0,-8,8), vec3(0,0,0), vec3(0,0,1));
	camera->setProjection(projection);
}

void MainWindow::initCube()
{
	GLfloat vertices[] =
	{
		// bottom
		1.0f, 1.0f,-1.0f,	-1.0f,-1.0f,-1.0f,	-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,	1.0f,-1.0f,-1.0f,	-1.0f,-1.0f,-1.0f,
		// top
		-1.0f, 1.0f, 1.0f,	-1.0f,-1.0f, 1.0f,	1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,	-1.0f, 1.0f, 1.0f,	1.0f,-1.0f, 1.0f,
		// left
		-1.0f,-1.0f,-1.0f,	-1.0f,-1.0f, 1.0f,	-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,	-1.0f, 1.0f, 1.0f,	-1.0f, 1.0f,-1.0f,
		// right
		1.0f, 1.0f, 1.0f,	1.0f,-1.0f,-1.0f,	1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,	1.0f, 1.0f, 1.0f,	1.0f,-1.0f, 1.0f,
		// front
   		1.0f,-1.0f, 1.0f,	-1.0f,-1.0f,-1.0f,	1.0f,-1.0f,-1.0f,
   		1.0f,-1.0f, 1.0f,	-1.0f,-1.0f, 1.0f,	-1.0f,-1.0f,-1.0f,
		// back
   		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,-1.0f,	-1.0f, 1.0f,-1.0f,
   		1.0f, 1.0f, 1.0f,	-1.0f, 1.0f,-1.0f,	-1.0f, 1.0f, 1.0f,
	};

	GLfloat normals[] =
	{
		// bottom
		0., 0., -1.,	0., 0., -1.,		0., 0., -1.,
		0., 0., -1.,	0., 0., -1.,		0., 0., -1.,
		// top
		0., 0., 1.,		0., 0., 1.,			0., 0., 1.,
		0., 0., 1.,		0., 0., 1.,			0., 0., 1.,
		// left
		-1.,0.,0.,		-1.,0.,0.,			-1.,0.,0.,
		-1.,0.,0.,		-1.,0.,0.,			-1.,0.,0.,
		// right
		1.,0.,0.,		 1.,0.,0.,			1.,0.,0.,
		1.,0.,0.,		 1.,0.,0.,			1.,0.,0.,
		// front
		0., -1., 0.,	 0., -1., 0.,		0., -1., 0.,
		0., -1., 0.,	 0., -1., 0.,		0., -1., 0.,
		// back
		0., 1., 0.,		 0., 1., 0.,		0., 1., 0.,
		0., 1., 0.,		 0., 1., 0.,		0., 1., 0.,
	};

	GLuint indices[36];
	for (int i = 0; i < 36; i++)
		indices[i] = i;

	GLfloat uv[] =
	{
		// bottom
		0., 0.,		0., 0.,		0., 0.,
		0., 0.,		0., 0.,		0., 0.,
		// top
		.125, .0,	.125, .0,	.125, .0,
		.125, .0,	.125, .0,	.125, .0,
		// left
		.25, 0.,	.25, 0.,	.25, 0.,
		.25, 0.,	.25, 0.,	.25, 0.,
		// right
		.375, 0.,	.375, 0.,	.375, 0.,
		.375, 0.,	.375, 0.,	.375, 0.,
		// front
		.5, 0.,		.5, 0.,		.5, 0.,
		.5, 0.,		.5, 0.,		.5, 0.,
		// back
		.625, 0.,	.625, 0.,	.625, 0.,
		.625, 0.,	.625, 0.,	.625, 0.,
	};

	cubeMesh = new Mesh;
	cubeMesh->setVertices((vec3*)vertices, 36);
	cubeMesh->setIndices(indices, 36);
	cubeMesh->setTextureCoord((vec2*)uv, 36);
	cubeMesh->setNormals((vec3*)normals, 36);

	cube = new SceneMeshItem(cubeMesh);
	cube->setShader(shader);
	rootNode->addItem(cube);
}

void MainWindow::initPlate()
{
	GLfloat vertices[] =
	{
		// bottom
		1.0f, 1.0f,.0f,	-1.0f,-1.0f,.0f,	-1.0f, 1.0f,.0f,
		1.0f, 1.0f,.0f,	1.0f,-1.0f,.0f,	-1.0f,-1.0f,.0f,
		// top
		-1.0f, 1.0f, .0f,	-1.0f,-1.0f, .0f,	1.0f,-1.0f, .0f,
		1.0f, 1.0f, .0f,	-1.0f, 1.0f, .0f,	1.0f,-1.0f, .0f,
	};

	GLfloat normals[] =
	{
		// bottom
		0., 0., -1.,	0., 0., -1.,		0., 0., -1.,
		0., 0., -1.,	0., 0., -1.,		0., 0., -1.,
		// top
		0., 0., 1.,		0., 0., 1.,			0., 0., 1.,
		0., 0., 1.,		0., 0., 1.,			0., 0., 1.,
	};

	GLuint indices[12];
	for (int i = 0; i < 12; i++)
		indices[i] = i;

	GLfloat uv[] =
	{
		// bottom
		.75, .0,	.75, .0,	.75, .0,
		.75, .0,	.75, .0,	.75, .0,
		// top
		.75, .0,	.75, .0,	.75, .0,
		.75, .0,	.75, .0,	.75, .0,
	};

	plateMesh = new Mesh;
	plateMesh->setVertices((vec3*)vertices, 12);
	plateMesh->setIndices(indices, 12);
	plateMesh->setTextureCoord((vec2*)uv, 12);
	plateMesh->setNormals((vec3*)normals, 12);

	plate = new SceneMeshItem(plateMesh);
	plate->setShader(shader);
	rootNode->addItem(plate);
}

MainWindow::~MainWindow()
{
	delete shader;
	delete texture;
	delete camera;
	delete rootNode;
	delete cubeMesh;
	delete cube;
	delete plateMesh;
	delete plate;
}

void MainWindow::render()
{
	renderer.clearScreen();
	renderer.initFrame();

	static float val = 0;
	val += 0.004;
	Transform cubeTransform;
	cubeTransform.scale(vec3(2, 1, 2));
	cubeTransform.rotate(vec3(0, 0, val*3.14f));
	cube->setTransform(cubeTransform);

	Transform plateTransform;
	plateTransform.scale(vec3(5, 5, 1));
	plateTransform.translate(vec3(0, 0, -3));
	plate->setTransform(plateTransform);

	rootNode->render(camera);
}

void MainWindow::update()
{
	static vec2 lastMousePos = getMousePos();

	vec2 mousePos = getMousePos();
	if(getMouseState(Button::ButtonLeft) == true)
	{
		vec2 diff = mousePos - lastMousePos;

		Quatf rotateVector = diff.x * camera->getUp() + diff.y * camera->getRight();
		Quatf rotor = exp(-0.003 * rotateVector);

		vec3 direction = rotor * camera->getDirection() * ~rotor;
		vec3 position = rotor * camera->getPosition() * ~rotor;
		camera->setDirection(direction);
		camera->setPosition(position);
	}
	lastMousePos = mousePos;
}

int main()
{
	MainWindow window;
	window.startLoop();
	window.terminate();

	return 0;
}
