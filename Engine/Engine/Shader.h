#pragma once

#include <fstream>
#include <sstream>

#include "GLEW/glew.h"
#include "GLM/glm.hpp"
#include "Debug.h"

class Shader
{
public:
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

	// activate the shader
	void use();

	// utility uniform functions
	void setUniform(const std::string &name, bool value);
	void setUniform(const std::string &name, int value);
	void setUniform(const std::string &name, float value);
	void setUniform(const std::string &name, const glm::vec2 &value);
	void setUniform(const std::string &name, float x, float y);
	void setUniform(const std::string &name, const glm::vec3 &value);
	void setUniform(const std::string &name, float x, float y, float z);
	void setUniform(const std::string &name, const glm::vec4 &value);
	void setUniform(const std::string &name, float x, float y, float z, float w);
	void setUniform(const std::string &name, const glm::mat2 &mat);
	void setUniform(const std::string &name, const glm::mat3 &mat);
	void setUniform(const std::string &name, const glm::mat4 &mat);

private:
	// utility function for checking shader compilation/linking errors.
	void checkCompileErrors(GLuint shader, std::string type);
};
