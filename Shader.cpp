#include "shader.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	// Retrive vertex and fragment shader source codes fram filepath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// Ensure ifstream can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n" << std::endl;
	}
	std::cout << "SHADER::READ::SUCCESSFULLY\n" << std::endl;
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	// compile shader
	unsigned int vertexShader, fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	int success;
	char infoLog[512];
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPIILE::FAIL\n" << infoLog << std::endl;
	}

	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILE::FAIL\n" << infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
#ifdef _DEBUG_
		std::cout << "ERROR::SHADER::PROGRAM::LINK::FAIL\n" << infoLog << std::endl;
#endif
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
#ifdef _DEBUG_
	std::cout << "SHADER::INITILIZATION::SUCCESSFUL\n" << std::endl;
#endif
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, GLboolean value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, GLint value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, GLfloat value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec3(const std::string& name, glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setMat4(const std::string& name, glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}