#include <text.h>
#include <shader.h>

int FreeTypeEngine::initFreeType()
{
	if(FT_Init_FreeType(&ft)) { fprintf(stderr, "ERROR: Could not initialize FreeType library\n"); return 1; }
	if(FT_New_Face(ft, "FreeSans.ttf", 0, &face)) { fprintf(stderr, "ERROR: Could not open font\n"); return 1; }
	FT_Set_Pixel_Sizes(face, 0, 16);
	g = face->glyph;
	return 0;
}

void FreeTypeEngine::load()
{
	loadProgram("glsl/text_vertex.glsl", "glsl/text_fragment.glsl");
	uniform_tex 	= glGetUniformLocation(Program, "tex");
	color 			= glGetUniformLocation(Program, "color");
	attribute_coord = glGetAttribLocation(Program, "coord");

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(uniform_tex, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenBuffers(1, &vbo);
	glEnableVertexAttribArray(attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void FreeTypeEngine::render(const char *text, float x, float y, float sx, float sy) {
  const char *p;

	glUseProgram(Program);

	GLfloat black[4] = {0, 0, 0, 1};
    glUniform4fv(color, 1, black);

  	/*glEnableVertexAttribArray(coord);
  	glBindBuffer(GL_ARRAY_BUFFER, vbo);
  	glVertexAttribPointer(coord, 4, GL_FLOAT, GL_FALSE, 0, 0);*/

  for(p = text; *p; p++) {
    if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
        continue;

    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RED,
      g->bitmap.width,
      g->bitmap.rows,
      0,
      GL_RED,
      GL_UNSIGNED_BYTE,
      g->bitmap.buffer
    );

    float x2 = x + g->bitmap_left * sx;
    float y2 = -y - g->bitmap_top * sy;
    float w = g->bitmap.width * sx;
    float h = g->bitmap.rows * sy;

    GLfloat box[4][4] = {
        {x2,     -y2    , 0, 0},
        {x2 + w, -y2    , 1, 0},
        {x2,     -y2 - h, 0, 1},
        {x2 + w, -y2 - h, 1, 1},
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    x += (g->advance.x >> 6) * sx;
    y += (g->advance.y >> 6) * sy;
  }
}
