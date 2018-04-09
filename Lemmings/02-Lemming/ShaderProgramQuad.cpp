#include <glm/gtc/type_ptr.hpp>
#include "ShaderProgramQuad.h"


ShaderProgramQuad::ShaderProgramQuad()
{
	programId = 0;
	linked = false;
}


void ShaderProgramQuad::init()
{
	programId = glCreateProgram();
}

void ShaderProgramQuad::addShader(const Shader &shader)
{
	glAttachShader(programId, shader.getId());
}

void ShaderProgramQuad::bindFragmentOutput(const string &outputName)
{
	glBindAttribLocation(programId, 0, outputName.c_str());
}

GLint ShaderProgramQuad::bindVertexAttribute(const string &attribName, GLint size)
{
	GLint attribPos;

	attribPos = glGetAttribLocation(programId, attribName.c_str());
	glVertexAttribPointer(attribPos, size, GL_FLOAT, GL_FALSE, 0, 0);

	return attribPos;
}

void ShaderProgramQuad::link()
{
	GLint status;
	char buffer[512];

	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &status);
	linked = (status == GL_TRUE);
	glGetProgramInfoLog(programId, 512, NULL, buffer);
	errorLog.assign(buffer);
}

void ShaderProgramQuad::free()
{
	glDeleteProgram(programId);
}

void ShaderProgramQuad::use()
{
	glUseProgram(programId);
}

bool ShaderProgramQuad::isLinked()
{
	return linked;
}

const string &ShaderProgramQuad::log() const
{
	return errorLog;
}

void ShaderProgramQuad::setUniform2f(const string &uniformName, float v0, float v1)
{
	GLint location = glGetUniformLocation(programId, uniformName.c_str());

	if (location != -1)
		glUniform2f(location, v0, v1);
}

void ShaderProgramQuad::setUniform3f(const string &uniformName, float v0, float v1, float v2)
{
	GLint location = glGetUniformLocation(programId, uniformName.c_str());

	if (location != -1)
		glUniform3f(location, v0, v1, v2);
}

void ShaderProgramQuad::setUniform4f(const string &uniformName, float v0, float v1, float v2, float v3)
{
	GLint location = glGetUniformLocation(programId, uniformName.c_str());

	if (location != -1)
		glUniform4f(location, v0, v1, v2, v3);
}

void ShaderProgramQuad::setUniformMatrix4f(const string &uniformName, glm::mat4 &mat)
{
	GLint location = glGetUniformLocation(programId, uniformName.c_str());

	if (location != -1)
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

