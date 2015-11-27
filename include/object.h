#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <iostream>
#include <GL/glew.h>

typedef GLuint VBO;

class Object
{
public:
	GLuint VAO;
	void init() { glGenVertexArrays(1, &VAO); genBuffers(); }
	~Object() { deleteBuffers(); glDeleteVertexArrays(1, &VAO);}
	virtual void genBuffers() {}
	virtual void deleteBuffers() {}
};

#endif // OBJECT_H_INCLUDED
