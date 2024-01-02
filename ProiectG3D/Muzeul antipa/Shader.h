#include <stdlib.h> // necesare pentru citirea shaderStencilTesting-elor
#include <stdio.h>
#include <math.h> 

#include <GL/glew.h>

#define GLM_FORCE_CTOR_INIT 
#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#pragma once
class Shader
{
private:
	unsigned int ID;
	void Init(const char* vertexPath, const char* fragmentPath);
	void CheckCompileErrors(unsigned int shaderStencilTesting, std::string type);
public:
	unsigned int loc_model_matrix;
	unsigned int loc_view_matrix;
	unsigned int loc_projection_matrix;
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	void Use();
	unsigned int GetID();
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetVec3(const std::string& name, float x, float y, float z) const;
	void SetMat4(const std::string& name, const glm::mat4& mat) const;
};

