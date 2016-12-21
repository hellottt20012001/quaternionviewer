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
	camera->lookAt(vec3(0,-8,8), vec3(0,0,0), vec3(0,0,1));
	camera->setProjection(projection);
}

void MainWindow::initMeshItem()
{
	bool result = true;
	shader = new ThreeDShader;
	result &= shader->addVertexShader(File::readAllText("data/shader/test.vs"));
	result &= shader->addFragmentShader(File::readAllText("data/shader/BasicLighting.fs"));
	result &= shader->compile();
	if(!result)
	{
		terminate();
		exit(0);
	}

	shader->Shader::bind();
	shader->setParameter("pointLight.position",vec3(0, -10, 10));
	shader->setParameter("pointLight.radiant",vec3(50.0f));
	shader->setParameter("ambient",vec3(10.f/255));
	shader->Shader::unbind();

	GLfloat vertices[] =
	{
		1.0f, 1.0f,-1.0f,	-1.0f,-1.0f,-1.0f,	-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,	1.0f,-1.0f,-1.0f,	-1.0f,-1.0f,-1.0f,

		-1.0f, 1.0f, 1.0f,	-1.0f,-1.0f, 1.0f,	1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,	-1.0f, 1.0f, 1.0f,	1.0f,-1.0f, 1.0f,

		-1.0f,-1.0f,-1.0f,	-1.0f,-1.0f, 1.0f,	-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,	-1.0f, 1.0f, 1.0f,	-1.0f, 1.0f,-1.0f,

		1.0f, 1.0f, 1.0f,	1.0f,-1.0f,-1.0f,	1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,	1.0f, 1.0f, 1.0f,	1.0f,-1.0f, 1.0f,

   		1.0f,-1.0f, 1.0f,	-1.0f,-1.0f,-1.0f,	1.0f,-1.0f,-1.0f,
   		1.0f,-1.0f, 1.0f,	-1.0f,-1.0f, 1.0f,	-1.0f,-1.0f,-1.0f,

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

	vec4 colors[4] =
	{
		vec4(1, 1, 0, 1),
		vec4(255, 0, 255, 255),
		vec4(0, 255, 255, 255),
		vec4(255, 255, 255, 255)
	};

	//Image image;
	//image.load("data/texture/planks_oak.png");
	texture = new Texture;
	//texture->load(&image);
	texture->fromRaw(colors, 2, 2);
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
