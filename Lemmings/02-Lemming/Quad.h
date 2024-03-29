#ifndef _QUAD_INCLUDE
#define _QUAD_INCLUDE


#include "ShaderProgramQuad.h"


// Given a point (x, y) and a size (width, height) in pixels Quad creates 
// a VBO with two triangles. Vertices in the VBO have only a position
// attribute (no colors or texture coordinates)


class Quad
{

public:
	// Quads can only be created inside an OpenGL context
	static Quad *createQuad(float x, float y, float width, float height, ShaderProgramQuad &program);

	Quad(float x, float y, float width, float height, ShaderProgramQuad &program);

	void render() const;
	void free();

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation;

};


#endif // _QUAD_INCLUDE
