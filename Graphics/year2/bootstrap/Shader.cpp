#include "gl_core_4_4.h"
#include "Mesh.h"
#include "Shader.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>


Shader::Shader()
{
}
Shader::~Shader()
{
}
void Shader::bind()
{
	glUseProgram(m_program);
}
void Shader::unbind()
{
	glUseProgram(0);
}
void Shader::load(const char* filename, unsigned int type)
{
	char previousDir[MAX_PATH];
	char* newDir = "..//bin//shaders";
	GetCurrentDirectory(MAX_PATH, previousDir);
	SetCurrentDirectory(newDir);

	std::fstream codeStream;

	std::string tmp = "";
	std::string source = "";

	int vertSuccess = GL_FALSE;
	int fragSuccess = GL_FALSE;

	switch (type)
	{
	case GL_VERTEX_SHADER:
		tmp = "";
		source = "";
		codeStream.open(filename);
		while (codeStream.eof() == false)
		{
			std::getline(codeStream, tmp);
			source += tmp + "\n";
		}
		vsSource = source.c_str();

		m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_vertexShader, 1, (const char**)&vsSource, 0);
		glCompileShader(m_vertexShader);
		glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &vertSuccess);
		if (vertSuccess == GL_FALSE)
		{
			int logLength = 0;
			glGetShaderiv(m_vertexShader, GL_INFO_LOG_LENGTH, &logLength);
			char* infoLog = new char[logLength];
			glGetShaderInfoLog(m_vertexShader, logLength, 0, infoLog);
			printf("VERTEX SHADER COMPILE ERROR\n");
			printf("%s\n", infoLog);
			delete[] infoLog;
		}
		break;

	case GL_FRAGMENT_SHADER:
		tmp = "";
		source = "";
		codeStream.open(filename);
		while (codeStream.eof() == false)
		{
			std::getline(codeStream, tmp);
			source += tmp + "\n";
		}
		fsSource = source.c_str();

		m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_fragmentShader, 1, (const char**)&fsSource, 0);
		glCompileShader(m_fragmentShader);
		glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &fragSuccess);
		if (fragSuccess == GL_FALSE)
		{
			int logLength = 0;
			glGetShaderiv(m_fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
			char* infoLog = new char[logLength];
			glGetShaderInfoLog(m_fragmentShader, logLength, 0, infoLog);
			printf("FRAGMENT SHADER COMPILE ERROR\n");
			printf("%s\n", infoLog);
			delete[] infoLog;
		}
		break;
	}

	SetCurrentDirectory(previousDir);
}
void Shader::attach()
{
	m_program = glCreateProgram();
	glAttachShader(m_program, m_vertexShader);
	glAttachShader(m_program, m_fragmentShader);
	glLinkProgram(m_program);
	int success = GL_FALSE;
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);

	if (success == GL_FALSE)
	{
		int infoLogLength = 10;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength + 1];
		glGetProgramInfoLog(m_program, infoLogLength, 0, infoLog);
		delete[] infoLog;
	}
}
void Shader::defaultLoad()
{
	vsSource = "#version 410\n \
							layout(location = 0) in vec4 Position; \
							layout(location = 1) in vec4 Color; \
							out vec4 vColor; \
							uniform mat4 ProjectionViewWorld; \
							void main() { vColor = Color; \
							gl_Position = ProjectionViewWorld * Position; }";
	fsSource = "#version 410\n \
							in vec4 vColor; \
							out vec4 FragColor; \
							void main() {FragColor = vColor; }";
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, (const char **)&vsSource, 0);
	glCompileShader(vertexShader);
	int vertSuccess = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertSuccess);
	if (vertSuccess == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_program, infoLogLength, 0, infoLog);
		printf("Error: Failed to compile vertex shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);
	int fragSuccess = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragSuccess);
	if (fragSuccess == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_program, infoLogLength, 0, infoLog);
		printf("Error: Failed to compile fragment shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	m_program = glCreateProgram();

	glAttachShader(m_program, vertexShader);
	glAttachShader(m_program, fragmentShader);
	glLinkProgram(m_program);

	int success = GL_FALSE;
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_program, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
}
unsigned int Shader::getUniform(const char * name)
{
	return glGetUniformLocation(m_program, name);
}