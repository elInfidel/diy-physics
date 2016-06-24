#pragma once

class Shader {
public:

	Shader() : m_program(0), m_error(nullptr) {
		m_shaders[0] = m_shaders[1] = m_shaders[2] = m_shaders[3] = m_shaders[4] = 0;
	}
	~Shader();

	bool loadShader(unsigned int type, const char* filename);
	bool createShader(unsigned int type, const char* string);

	bool link();

	void bind();

	int getUniform(const char* name);

	unsigned int getHandle() const { return m_program; }

	const char* getLastError() const { return m_error; }

private:

	unsigned int	m_program;
	// vert, cont, eval, geom, frag
	unsigned int	m_shaders[5];
	char*			m_error;
};
