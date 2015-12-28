#include <quatview.h>

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
		// white
		1.0f,  1.0f,  1.0f,		1.0f,  1.0f,  1.0f,		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,		1.0f,  1.0f,  1.0f,		1.0f,  1.0f,  1.0f,
		// yellow
		255.0/255.0,  213.0/255.0,  0.0/255.0,		255.0/255.0,  213.0/255.0,  0.0/255.0,		255.0/255.0,  213.0/255.0,  0.0/255.0,
		255.0/255.0,  213.0/255.0,  0.0/255.0,		255.0/255.0,  213.0/255.0,  0.0/255.0,		255.0/255.0,  213.0/255.0,  0.0/255.0,
		// orange
		255.0/255.0,  88.0/255.0,  0.0/255.0,		255.0/255.0,  88.0/255.0,  0.0/255.0,		255.0/255.0,  88.0/255.0,  0.0/255.0,
		255.0/255.0,  88.0/255.0,  0.0/255.0,		255.0/255.0,  88.0/255.0,  0.0/255.0,		255.0/255.0,  88.0/255.0,  0.0/255.0,
		// red
 		196.0/255.0,  30.0/255.0,  58.0/255.0,		196.0/255.0,  30.0/255.0,  58.0/255.0,		196.0/255.0,  30.0/255.0,  58.0/255.0,
		196.0/255.0,  30.0/255.0,  58.0/255.0,		196.0/255.0,  30.0/255.0,  58.0/255.0,		196.0/255.0,  30.0/255.0,  58.0/255.0,
		// green
		0.0/255.0,  158.0/255.0,  96.0/255.0,		0.0/255.0,  158.0/255.0,  96.0/255.0,		0.0/255.0,  158.0/255.0,  96.0/255.0,
		0.0/255.0,  158.0/255.0,  96.0/255.0,		0.0/255.0,  158.0/255.0,  96.0/255.0,		0.0/255.0,  158.0/255.0,  96.0/255.0,
		// blue
		0.0/255.0,  81.0/255.0,  186.0/255.0,	0.0/255.0,  81.0/255.0,  186.0/255.0,		0.0/255.0,  81.0/255.0,  186.0/255.0,
		0.0/255.0,  81.0/255.0,  186.0/255.0,	0.0/255.0,  81.0/255.0,  186.0/255.0,		0.0/255.0,  81.0/255.0,  186.0/255.0
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
		0.0f,  1.0f,  0.0f,		0.0f,  1.0f,  0.0f,		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,		0.0f,  1.0f,  0.0f,		0.0f,  1.0f,  0.0f
	};
	vertexColorData(plateColor);
}

int Data::readFromCSV(const char* csv_filepath)
{
	std::ifstream csvStream(csv_filepath, std::ios::in);
	if(csvStream.is_open())
    {
		std::string line; std::stringstream lineStream;
		auto readVal = [&] () { std::string val; std::getline(lineStream, val , ','); return std::stringstream(val); };

		std::getline(csvStream, line);
		lineStream = std::stringstream(line);
		std::vector<std::string> keys; std::string key;
		while(std::getline(lineStream, key , ',')) keys.push_back(key);

		while(std::getline(csvStream, line))
		{
			lineStream = std::stringstream(line);
			DataLine dataLine;
			float w, x, y, z;

			for(unsigned int k = 0; k < keys.size(); k++)
			{
				if(keys[k].compare("Time") == 0)
				{
					std::string time;
					readVal() >> time;
					std::stringstream timeStream(time);
					auto readTime = [&] () { std::string val; std::getline(timeStream, val , ':'); return std::stringstream(val); };
					int hr, min, sec;
					readTime() >> hr; readTime() >> min; readTime() >> sec;
					dataLine.Time = hr * 3600 + min * 60 + sec;
				}
				else if(keys[k].compare("Package N.O") == 0) readVal() >> dataLine.Package_NO;
				else if(keys[k].compare("Outside Temperature") == 0)	readVal() >> dataLine.OutsideTemperature;
				else if(keys[k].compare("Temperature") == 0) readVal() >> dataLine.Temperature;
				else if(keys[k].compare("Pressure") == 0)	readVal() >> dataLine.Pressure;
				else if(keys[k].compare("W") == 0) readVal() >> w;
				else if(keys[k].compare("X") == 0) readVal() >> x;
				else if(keys[k].compare("Y") == 0) readVal() >> y;
				else if(keys[k].compare("Z") == 0) readVal() >> z;
				else if(keys[k].compare("Voltage") == 0) readVal() >> dataLine.Voltage;
				else if(keys[k].compare("RSSI") == 0) readVal() >> dataLine.RSSI;
				else if(keys[k].compare("Frequency Error") == 0) readVal() >> dataLine.Frequency;
				else if(keys[k].compare("CRC") == 0) readVal() >> dataLine.CRC;
			}
			dataLine.Orientation = quat(w, x, y, z);
			item.push_back(dataLine);
		}
		csvStream.close(); std::cout << "Loaded \"" << csv_filepath << "\"\n\n"; return 1;
	} else std::cout << "Failed to load \"" << csv_filepath << "\"\n\n"; return 0;
}
