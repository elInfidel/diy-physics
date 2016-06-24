#include "Shader.h"
#include "loadgen/gl_core_4_4.h"
#include <stdio.h>

Shader::~Shader() {
	delete[] m_error;
	glDeleteProgram(m_program);
	for (auto& s : m_shaders)
		glDeleteShader(s);
}

bool Shader::loadShader(unsigned int type, const char* filename) {

	unsigned int* shader = nullptr;

	switch (type) {
	case GL_VERTEX_SHADER: shader = &m_shaders[0];	break;
	case GL_TESS_CONTROL_SHADER: shader = &m_shaders[1];	break;
	case GL_TESS_EVALUATION_SHADER: shader = &m_shaders[2];	break;
	case GL_GEOMETRY_SHADER: shader = &m_shaders[3];	break;
	case GL_FRAGMENT_SHADER: shader = &m_shaders[4];	break;
	default: printf("INVALID SHADER TYPE SELECTED!\n");	return false;
	}

	*shader = glCreateShader(type);

	// open file
	FILE* file = nullptr;
	fopen_s(&file, filename, "rb");
	fseek(file, 0, SEEK_END);
	unsigned int size = ftell(file);
	char* source = new char[size + 1];
	fseek(file, 0, SEEK_SET);
	fread_s(source, size + 1, sizeof(char), size, file);
	fclose(file);
	source[size] = 0;

	glShaderSource(*shader, 1, (const char**)&source, 0);
	glCompileShader(*shader);

	delete[] source;

	int success = GL_TRUE;
	glGetShaderiv(*shader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		delete[] m_error;
		m_error = new char[infoLogLength];
		glGetShaderInfoLog(*shader, infoLogLength, 0, m_error);
		return false;
	}

	return true;
}

bool Shader::createShader(unsigned int type, const char* string) {

	unsigned int* shader = nullptr;

	switch (type) {
	case GL_VERTEX_SHADER: shader = &m_shaders[0];	break;
	case GL_TESS_CONTROL_SHADER: shader = &m_shaders[1];	break;
	case GL_TESS_EVALUATION_SHADER: shader = &m_shaders[2];	break;
	case GL_GEOMETRY_SHADER: shader = &m_shaders[3];	break;
	case GL_FRAGMENT_SHADER: shader = &m_shaders[4];	break;
	default: printf("INVALID SHADER TYPE SELECTED!\n");	return false;
	}

	*shader = glCreateShader(type);

	glShaderSource(*shader, 1, (const char**)&string, 0);
	glCompileShader(*shader);
	
	int success = GL_TRUE;
	glGetShaderiv(*shader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		delete[] m_error;
		m_error = new char[infoLogLength];
		glGetShaderInfoLog(*shader, infoLogLength, 0, m_error);
		return false;
	}

	return true;
}

bool Shader::link() {
	m_program = glCreateProgram();
	for (auto& s : m_shaders)
		glAttachShader(m_program, s);
	glLinkProgram(m_program);

	int success = GL_TRUE;
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);

		delete[] m_error;
		m_error = new char[infoLogLength];
		glGetProgramInfoLog(m_program, infoLogLength, 0, m_error);
		return false;
	}
	return true;
}

void Shader::bind() {
	glUseProgram(m_program);
}

int Shader::getUniform(const char* name) {
	return glGetUniformLocation(m_program, name);
}