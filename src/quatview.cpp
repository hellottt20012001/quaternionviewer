#include <quatview.h>
#include <iostream>

void QuatViewProgram::load()
{
	loadProgram("glsl/vertex.glsl", "glsl/fragment.glsl");
	VertexXYZ 	= glGetAttribLocation(Program, "VertexXYZ");
	VertexColor = glGetAttribLocation(Program, "VertexColor");
	MVP 		= glGetUniformLocation(Program, "MVP");
	orientation = glGetUniformLocation(Program, "orientation");

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);   // Accept fragment if it closer to the camera than the former one
}

void QuatViewProgram::draw(QuatViewObject object, mat4 mvp)
{
	// Use program
	glUseProgram(Program);

	// uniform: orientation
	glUniform4f(orientation, object.orientation.x, object.orientation.y, object.orientation.z, object.orientation.w);

	// uniform: MVP
	glUniformMatrix4fv(MVP, 1, GL_FALSE, &(mvp)[0][0]);

	// 1st attribute : VertexXYZ
	glEnableVertexAttribArray(VertexXYZ);
	glBindBuffer(GL_ARRAY_BUFFER, object.VertexXYZ);
	glVertexAttribPointer(VertexXYZ, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute : VertexColor
	glEnableVertexAttribArray(VertexColor);
	glBindBuffer(GL_ARRAY_BUFFER, object.VertexColor);
	glVertexAttribPointer(VertexColor, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, object.n_vert);
}

void QuatViewObject::genBuffers()
{
	glBindVertexArray(VAO);
	glGenBuffers(1, &VertexXYZ);
	glGenBuffers(1, &VertexColor);
}

void QuatViewObject::deleteBuffers()
{
	glDeleteBuffers(1, &VertexXYZ);
	glDeleteBuffers(1, &VertexColor);
}

void QuatViewObject::vertexXYZData(GLfloat data[])
{
	glBindBuffer(GL_ARRAY_BUFFER, VertexXYZ);
	glBufferData(GL_ARRAY_BUFFER, n_vert * 3 * sizeof(data[0]), data, GL_STATIC_DRAW);
}

void QuatViewObject::vertexColorData(GLfloat data[])
{
	glBindBuffer(GL_ARRAY_BUFFER, VertexColor);
	glBufferData(GL_ARRAY_BUFFER, n_vert * 3 * sizeof(data[0]), data, GL_STATIC_DRAW);
}

void QuatViewObject::createCube()
{
	init();
	n_vert = 12 * 3;
	GLfloat cubeXYZ[] = {
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
	vertexXYZData(cubeXYZ);
	GLfloat cubeColor[] = {
		// red
 		1.0f,  0.0f,  0.0f,		1.0f,  0.0f,  0.0f,		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,		1.0f,  0.0f,  0.0f,		1.0f,  0.0f,  0.0f,
		// cyan
		0.0f,  1.0f,  1.0f,		0.0f,  1.0f,  1.0f,		0.0f,  1.0f,  1.0f,
		0.0f,  1.0f,  1.0f,		0.0f,  1.0f,  1.0f,		0.0f,  1.0f,  1.0f,
		// green
		0.0f,  1.0f,  0.0f,		0.0f,  1.0f,  0.0f,		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,		0.0f,  1.0f,  0.0f,		0.0f,  1.0f,  0.0f,
		// magenta
		1.0f,  0.0f,  1.0f,		1.0f,  0.0f,  1.0f,		1.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  1.0f,		1.0f,  0.0f,  1.0f,		1.0f,  0.0f,  1.0f,
		// blue
		0.0f,  0.0f,  1.0f,		0.0f,  0.0f,  1.0f,		0.0f,  0.0f,  1.0f,
		0.0f,  0.0f,  1.0f,		0.0f,  0.0f,  1.0f,		0.0f,  0.0f,  1.0f,
		// yellow
		1.0f,  1.0f,  0.0f,		1.0f,  1.0f,  0.0f,		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,		1.0f,  1.0f,  0.0f,		1.0f,  1.0f,  0.0f
	};
	vertexColorData(cubeColor);
}

void QuatViewObject::createPlate()
{
	init();
	n_vert = 2 * 3;
	GLfloat plateXYZ[] = {
		2.0f, 2.0f,-2.0f,		-2.0f,-2.0f,-2.0f,		-2.0f, 2.0f,-2.0f,
		2.0f, 2.0f,-2.0f,		2.0f,-2.0f,-2.0f,		-2.0f,-2.0f,-2.0f
	};
	vertexXYZData(plateXYZ);
	GLfloat plateColor[] = {
		// white
		1.0f,  1.0f,  1.0f,		1.0f,  1.0f,  1.0f,		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,		1.0f,  1.0f,  1.0f,		1.0f,  1.0f,  1.0f
	};
	vertexColorData(plateColor);
}

int Data::readFromCSV(const char* csv_filepath)
{
	std::ifstream csvStream(csv_filepath, std::ios::in);
	if(csvStream.is_open())
    {
		std::string line;
		getline(csvStream, line);
		while(getline(csvStream, line))
		{
			std::stringstream lineStream(line);
			DataLine dataLine;
			auto readVal = [&] () { std::string val; std::getline(lineStream, val , ','); return std::stringstream(val); };
			readVal() >> dataLine.Time;
			readVal() >> dataLine.Package_NO;
			readVal() >> dataLine.Temperature;
			readVal() >> dataLine.Pressure;
			float w, x, y, z;
			readVal() >> w; readVal() >> x; readVal() >> y; readVal() >> z;
			dataLine.Orientation = quat(w, x, y, z);
			readVal() >> dataLine.Voltage;
			readVal() >> dataLine.RSSI;
			readVal() >> dataLine.Frequency;
			readVal() >> dataLine.CRC;
			item.push_back(dataLine);
		}
		csvStream.close(); std::cout << "Loaded \"" << csv_filepath << "\"\n\n"; return 1;
	} else std::cout << "Failed to load \"" << csv_filepath << "\"\n\n"; return 0;
}
