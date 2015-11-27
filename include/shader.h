#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

typedef GLuint attrib;
typedef GLuint uniform;

class ShaderProgram
{
public:
	GLuint Program;

	int loadShader(const char* shader_filepath, GLuint &shader);
	int loadProgram(const char * vertex_filepath, const char * fragment_filepath);
	virtual void load() {}
	~ShaderProgram() { glDeleteProgram(Program); }
};

#endif
