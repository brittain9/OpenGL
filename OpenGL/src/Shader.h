#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

// pass in file or string and compile as shader
// bind and unbind shader
// set uniforms for shader

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
	std::string m_FilePathVertex;
	std::string m_FilePathFragment;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath, const std::string& fragment);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);

private:
	ShaderProgramSource ReadShader(const std::string& filepath1, const std::string& filepath2);
	unsigned int CompileShaders(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& VertexShader, const std::string& FragmentShader);
	int GetUniformLocation(const std::string& name);
};

