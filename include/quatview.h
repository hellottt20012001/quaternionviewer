#ifndef QUATVIEW_H_INCLUDED
#define QUATVIEW_H_INCLUDED

#include <GL/glew.h>

#include <object.h>
#include <shader.h>
#include <vec.h>
#include <quaternion.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class QuatViewObject : Object
{
public:
	int n_vert;
	VBO VertexXYZ;
	VBO VertexColor;
	quat orientation = quat(1);
	void genBuffers();
	void deleteBuffers();
	void vertexXYZData(GLfloat data[]);
	void vertexColorData(GLfloat data[]);
	void createCube();
	void createPlate();
};

class QuatViewProgram : ShaderProgram
{
public:
	attrib VertexXYZ;
	attrib VertexColor;
	uniform orientation;
	uniform MVP;
	void load();
	void draw(QuatViewObject object, mat4 mvp);
	glm::mat4 genMVP();
};

class DataLine
{
public:
	int Time;
	int Package_NO;
	float Temperature;
	float Pressure;
	quat Orientation;
	float Voltage;
	int RSSI;
	float Frequency;
	int CRC;
};

class Data
{
public:
	std::vector<DataLine> item;
	DataLine& operator[] (int i) { return item[i]; }
	int size() { return item.size(); }
	int readFromCSV(const char* csv_filepath);
};

#endif // QUATVIEW_H_INCLUDED
