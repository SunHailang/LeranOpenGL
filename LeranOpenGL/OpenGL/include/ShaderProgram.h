#pragma once

#include <iostream>
#include <unordered_map>


struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class ShaderProgram
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	ShaderProgram(const std::string& filePath);
	~ShaderProgram();

	void Bind() const;
	void Unbind() const;


	// set uniform
	void SetUniform1i(const std::string& name, int value);

	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v3, float v4);

private:
	int GetUniformLocation(const std::string& name);

	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};