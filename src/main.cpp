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
	ThreeDShader* shader;
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

	camera = new Camera();
	camera->lookAt(vec3(0,-5,5), vec3(0,0,0), vec3(0,0,1));
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

	GLfloat vertices[] =
	{
		// top
		-1., 1., 1.,		-1.,-1., 1.,		1.,-1., 1.,
		1., 1., 1.,		-1., 1., 1.,		1.,-1., 1.,
		// bottom
		1., 1.,-1.,		-1.,-1.,-1.,		-1., 1.,-1.,
		1., 1.,-1.,		 1.,-1.,-1.,		-1.,-1.,-1.,
		// left
		-1.,-1.,-1.,		-1., 1., 1.,		-1., 1.,-1.,
		-1.,-1.,-1.,		-1.,-1., 1.,		-1., 1., 1.,
		// right
		1.,-1.,-1.,		 1., 1., 1.,		1.,-1., 1.,
		1., 1., 1.,		 1.,-1.,-1.,		1., 1.,-1.,
		// front
		1., -1., 1.,		 1., -1.,-1.,		-1., -1.,-1.,
		1., -1., 1.,		-1., -1.,-1.,		-1., -1., 1.,
		// back
		1., 1., 1.,		-1.,  1., 1.,		-1.,1.,-1.,
		1., 1., 1.,		-1.,  1.,-1.,		1.,1.,-1.
	};

	GLfloat normals[36 * 3];
	for (int i = 0; i < 36; i++)
	{
		normals[i*3] = 0;
		normals[i*3 + 1] = 0;
		normals[i*3 + 2] = 1;
	}

	GLuint indices[36];
	for (int i = 0; i < 36; i++)
		indices[i] = i;

	GLfloat uv[36 * 2];
	for (int i = 0; i < 36; i++)
	{
		uv[i*2] 	= 0;
		uv[i*2+1] 	= 0;
	}

	mesh = new Mesh;
	mesh->setVertices((vec3*)vertices, 36);
	mesh->setIndices(indices, 36);
	mesh->setTextureCoord((vec2*)uv, 36);
	mesh->setNormals((vec3*)normals, 36);

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
	delete mesh;
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

	Transform transform;

	transform.scale(vec3(2,2,1));
	transform.rotate(vec3(0,val*3.14f,0));
	val += 0.004;

	rootNode->setTransform(transform);
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
		Quatf rotor = ~ exp(0.003 * rotateVector);

		vec3 direction =  rotor * camera->getDirection() * ~rotor;
		vec3 position =  rotor * camera->getPosition() * ~rotor;
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
