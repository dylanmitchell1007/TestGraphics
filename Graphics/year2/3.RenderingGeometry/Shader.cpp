#include <gl_core_4_4.h>
#include "Mesh.h"
#include "Shader.h"


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
	FILE* fp;
	errno_t success = fopen_s(&fp, filename, "rb");
	if (success < 0)
		perror("Cannot Find File");
	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	char*data = new char[size];
	fread_s(data, size, sizeof(char), size, fp);
	data[size] = '\0';
	fclose(fp);
	switch (type)
	{
	case GL_VERTEX_SHADER:
		m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
		vsSource = data;
		glShaderSource(m_vertexShader, 1, (const char **)&vsSource, 0);
		glCompileShader(m_vertexShader);

		break;
	case GL_FRAGMENT_SHADER:
		m_vertexShader = glCreateShader(GL_FRAGMENT_SHADER);
			fsSource = data;
		glShaderSource(m_fragmentShader, 1, (const char **)&fsSource, 0);
		glCompileShader(m_fragmentShader);

		break;
		
	}
	

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
							gl_Position = ProjectionViewWorld = Position) }";
	fsSource = "#version 410\n \
							in vec4 vColor; \
							out vec4 FragColor; \
							void main() {FragColor = vColor; }";
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, (const char **)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);

	m_program = glCreateProgram();

	glAttachShader(m_program, vertexShader);
	glAttachShader(m_program, fragmentShader);
	glLinkProgram(m_program);

	int success = GL_FALSE;
	int infoLogLength = 0;
	glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
	char* infoLog = new char[infoLogLength + 1];
	glGetProgramInfoLog(m_program, infoLogLength, 0, infoLog);
	delete[] infoLog;

	
}
unsigned int Shader::getUniform(const char * name)
	{
		return glGetUniformLocation(m_program, name);
	}