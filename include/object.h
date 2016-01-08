#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <iostream>
#include "gl_2_1.h"

typedef GLuint VBO;

class Object
{
public:
	GLuint VAO;
	void init() { genBuffers(); }
	~Object() { deleteBuffers(); }
	virtual void genBuffers() {}
	virtual void deleteBuffers() {}
};

#endif // OBJECT_H_INCLUDED
