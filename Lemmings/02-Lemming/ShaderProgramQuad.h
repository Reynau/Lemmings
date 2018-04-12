#ifndef _SHADER_PROGRAM_QUAD_INCLUDE
#define _SHADER_PROGRAM_QUAD_INCLUDE


#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include "Shader.h"


// Using the Shader class ShaderProgramQuad can link a vertex and a fragment shader
// together, bind input attributes to their corresponding vertex shader names, 
// and bind the fragment output to a name from the fragment shader


class ShaderProgramQuad
{

public:
	ShaderProgramQuad();

	void init();
	void addShader(const Shader &shader);
	void bindFragmentOutput(const string &outputName);
	GLint bindVertexAttribute(const string &attribName, GLint size);
	void link();
	void free();

	void use();

	// Pass uniforms to the associated shaders
	void setUniform2f(const string &uniformName, float v0, float v1);
	void setUniform3f(const string &uniformName, float v0, float v1, float v2);
	void setUniform4f(const string &uniformName, float v0, float v1, float v2, float v3);
	void setUniformMatrix4f(const string &uniformName, glm::mat4 &mat);

	bool isLinked();
	const string &log() const;

private:
	GLuint programId;
	bool linked;
	string errorLog;

};


#endif // _SHADER_PROGRAM_QUAD_INCLUDE