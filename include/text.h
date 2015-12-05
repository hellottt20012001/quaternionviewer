#ifndef TEXT_H
#define TEXT_H

#include <shader.h>

#include <ft2build.h>
#include FT_FREETYPE_H

class FreeTypeEngine : ShaderProgram
{
	FT_Library ft;
	FT_Face face;
	FT_GlyphSlot g;

	GLuint tex;
	GLuint vbo;

	uniform uniform_tex;
	uniform color;
	attrib attribute_coord;

public:
	int initFreeType();
	void load();
	void render(const char *text, float x, float y, float sx, float sy);
};

#endif // TEXT_H_INCLUDED
