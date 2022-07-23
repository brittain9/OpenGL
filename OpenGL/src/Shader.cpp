#include "Shader.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string& filepathVertex, const std::string& filepathFragment)
	: m_FilePathVertex(filepathVertex), m_FilePathFragment(filepathFragment), m_RendererID(0)
{
	ShaderProgramSource source = ReadShader(filepathVertex, filepathFragment);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCALL(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ReadShader(const std::string& filepathVertex, const std::string& filepathFragment)
{

	std::string line;
	std::stringstream ss[2];

	std::ifstream stream1(filepathVertex);


	while (getline(stream1, line))
	{

		ss[0] << line << '\n';
	}

	std::ifstream stream2(filepathFragment);

	while (getline(stream2, line))
	{

		ss[1] << line << '\n';
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShaders(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = &source[0];
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char)); // allocate on the stack dynamically.
		GLCALL(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile" <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< "shader!" << std::endl;
		std::cout << message << std::endl;

		GLCALL(glDeleteShader(id));
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& VertexShader, const std::string& FragmentShader)
{
	GLCALL(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShaders(GL_VERTEX_SHADER, VertexShader);
	unsigned int fs = CompileShaders(GL_FRAGMENT_SHADER, FragmentShader);

	GLCALL(glAttachShader(program, vs));
	GLCALL(glAttachShader(program, fs));
	GLCALL(glLinkProgram(program));
	GLCALL(glValidateProgram(program));

	GLCALL(glDeleteShader(vs));
	GLCALL(glDeleteShader(fs));

	return program;
}

void Shader::Bind() const
{
	GLCALL(glUseProgram(m_RendererID));
}
void Shader::Unbind() const
{
	GLCALL(glUseProgram(0));
}

// Set uniforms
void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCALL(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCALL(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4 matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}


int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLCALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
	{
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	}

	m_UniformLocationCache[name] = location;
	return location;
}

