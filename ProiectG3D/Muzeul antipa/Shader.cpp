#include "Shader.h"

void Shader::Init(const char* vertexPath, const char* fragmentPath)
{
}

void Shader::CheckCompileErrors(unsigned int shaderStencilTesting, std::string type)
{
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
}

Shader::~Shader()
{
}

void Shader::Use()
{
}

unsigned int Shader::GetID()
{
	return 0;
}

void Shader::SetInt(const std::string& name, int value) const
{
}

void Shader::SetFloat(const std::string& name, float value) const
{
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const
{
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
}
