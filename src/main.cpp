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

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

class MainWindow : public Window
{
public:
	MainWindow();
	~MainWindow();
	void render();

	void update();

protected:
	Renderer renderer;
	Mesh* mesh;
	Model* model;
	ThreeDShader* shader;
	ThreeDShader* normalShader;
	Texture* texture;
	Camera* camera;
	SceneNode* rootNode;
	SceneMeshItem* meshItem;

	void initMeshItem();
};

MainWindow::MainWindow()
{
	createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "PulsarEngine Example");

	renderer.init();
	renderer.setClearColor(vec3(135, 206, 250)/255.f);

	initMeshItem();

	Projection projection;
	projection.setProjection(radians(70.0f), WINDOW_WIDTH, WINDOW_HEIGHT, 0.01f, 50.0f);

	camera = new Camera(vec3(0,5,5), vec3(0,0,-1), vec3(0,1,0));
	camera->lookAt(vec3(0,5,5), vec3(0,0,0), vec3(0,1,0));
	camera->setProjection(projection);
}

void MainWindow::initMeshItem()
{
	bool result = true;
	shader = new ThreeDShader;
	result &= shader->addVertexShader(File::readAllText("data/shader/test.vs"));
	result &= shader->addFragmentShader(File::readAllText("data/shader/BasicLighting.fs"));
	result &= shader->compile();
	if(result == false)
	{
		terminate();
		exit(0);
	}

	shader->Shader::bind();
	shader->setParameter("pointLight.position",vec3(3, 2, 0));
	shader->setParameter("pointLight.radiant",vec3(3.0f));
	shader->Shader::unbind();

	const float d = 5;

	GLfloat vertices[] =
	{
		// top
		-1.0f, 1.0f, 0.5f,		-1.0f,-1.0f, 0.5f,		1.0f,-1.0f, 0.5f,
		1.0f, 1.0f, 0.5f,		-1.0f, 1.0f, 0.5f,		1.0f,-1.0f, 0.5f,
		// bottom
		1.0f, 1.0f,-0.5f,		-1.0f,-1.0f,-0.5f,		-1.0f, 1.0f,-0.5f,
		1.0f, 1.0f,-0.5f,		1.0f,-1.0f,-0.5f,		-1.0f,-1.0f,-0.5f,
		// left
		-1.0f,-1.0f,-0.5f,		-1.0f, 1.0f, 0.5f,		-1.0f, 1.0f,-0.5f,
		-1.0f,-1.0f,-0.5f,		-1.0f,-1.0f, 0.5f,		-1.0f, 1.0f, 0.5f,
		// right
		1.0f,-1.0f,-0.5f,		1.0f, 1.0f, 0.5f,		1.0f,-1.0f, 0.5f,
		1.0f, 1.0f, 0.5f,		1.0f,-1.0f,-0.5f,		1.0f, 1.0f,-0.5f,
		// front
		1.0f, -1.0f, 0.5f,		1.0f, -1.0f,-0.5f,		-1.0f, -1.0f,-0.5f,
		1.0f, -1.0f, 0.5f,		-1.0f, -1.0f,-0.5f,		-1.0f, -1.0f, 0.5f,
		// back
		1.0f, 1.0f, 0.5f,		-1.0f, 1.0f, 0.5f,		-1.0f,1.0f,-0.5f,
		1.0f, 1.0f, 0.5f,		-1.0f, 1.0f,-0.5f,		1.0f,1.0f,-0.5f
	};

	GLfloat normals[] =
	{
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
	};

	GLuint indices[] =
	{
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,
		24, 25, 26,
		27, 28, 29,
		30, 31, 32,
		33, 34, 35
	};

	GLfloat uv[] =
	{
		0,0,
		0,1,
		1,1,
		1,0,
		0,0,
		0,1,
		1,1,
		1,0,
		0,0,
		0,1,
		1,1,
		1,0
	};

	mesh = new Mesh;
	mesh->setVertices((vec3*)vertices, 36);
	mesh->setIndices(indices, 36);
	mesh->setTextureCoord((vec2*)uv, 12);
	mesh->setNormals((vec3*)normals, 12);

	Image image;
	image.load("data/texture/planks_oak.png");
	texture = new Texture;
	texture->load(&image);
	texture->enableMipmap(false);
	shader->setTexture(texture);

	rootNode = new SceneNode;
	meshItem = new SceneMeshItem(mesh);
	meshItem->setShader(shader);
	rootNode->addItem(meshItem);
}

MainWindow::~MainWindow()
{
	delete shader;
	delete normalShader;
	delete mesh;
	delete model;
	delete camera;
	delete texture;
	delete rootNode;
	delete meshItem;
}

void MainWindow::render()
{
	renderer.clearScreen();
	renderer.initFrame();

	static float val = 0;
	static float scal = 1;

	Transform transform;

	//transform.scale(vec3(1,1,1));
	//transform.translate(vec3(0,0,0));
	//transform.rotate(vec3(cos(val*3.14)*3.14,sin(val*3.14)*3.14,0));

	//transform.rotate(vec3(0,val*3.14f,0));

	rootNode->setTransform(transform);

	val += 0.004*scal;
	rootNode->render(camera);
}

void MainWindow::update()
{
	static vec2 lastMousePos = getMousePos();

	float moveSpeed = 0.025;
	//Move Camera
	if(getKeyState(Key::W) == true)
		camera->move(camera->getDirection()*moveSpeed);
	if(getKeyState(Key::S) == true)
		camera->move(-camera->getDirection()*moveSpeed);
	if(getKeyState(Key::D) == true)
		camera->move(camera->getRight()*moveSpeed);
	if(getKeyState(Key::A) == true)
		camera->move(-camera->getRight()*moveSpeed);
	if(getKeyState(Key::E) == true)
		camera->move(camera->getUp()*moveSpeed);
	if(getKeyState(Key::C) == true)
		camera->move(-camera->getUp()*moveSpeed);

	//if(getKeyState(Key::Esc) == true)
	// 	exit(0);

	//Mouse movements
	vec2 mousePos = getMousePos();
	if(getMouseState(Button::ButtonLeft) == true)
	{
		vec2 diff = mousePos - lastMousePos;

		Quatf rotateVector = diff.x * camera->getUp() + diff.y * camera->getRight();
		Quatf rotor = ~ exp(0.003 * rotateVector);

		vec3 direction =  rotor * camera->getDirection() * ~rotor;
		vec3 position =  rotor * camera->getPosition() * ~rotor;
		camera->setDirection(direction);
		camera->setPosition(position);

		//cout << direction.x << "\t" << direction.y << "\t" << direction.z << endl;
	}
	lastMousePos = mousePos;

	//cout << camera->getPosition().y << endl;
}

int main()
{
	MainWindow window;
	window.startLoop();
	window.terminate();

	return 0;
}
