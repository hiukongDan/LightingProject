#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
public:
	// Shader program ID
	GLuint ID;
	// Constructor
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// use and activate the program
	void use();
	void setBool(const std::string &name, GLboolean value) const;
	void setInt(const std::string &name, GLint value) const;
	void setFloat(const std::string &name, GLfloat value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	void setVec3(const std::string& name, glm::vec3 &value) const;
	void setMat4(const std::string& name, glm::mat4 &value) const;
};