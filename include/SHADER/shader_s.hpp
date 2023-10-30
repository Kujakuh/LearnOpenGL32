/*
*	SHADER_S.HPP
*	
*	This class is used to read and compile shaders
* 
*	The constructor will read and compile given shaders
* 
*	The use() function will activate the shader
* 
*	The set of setUniform functions will modify the value of the shader uniform
*	placed at the given location
* 
 */

#ifndef SHADER_S_H
#define SHADER_S_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // For glm::value_ptr

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


// Utils -----------------------------------------------------------------------
#pragma region "Shader construct utility functions"

void compileShader(GLuint shader, const char* shaderCode)
{
	int compilationFlag;
	char infoLog[512];

	glShaderSource(shader, 1, &shaderCode, NULL); // Attach shader source code to shader object
	glCompileShader(shader); // Compile the shader source code to the shader object

	// Check if compilation of the shader was successful :: snippet (Always the same)
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationFlag);
	if (!compilationFlag)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
	}
}

void linkShaderErrorCheck(GLuint shaderProgram)
{
	int linkFlag;
	char infoLog[512];

	// Check linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkFlag);
	if (!linkFlag)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
	}
}

std::string readShaderFile(const char* shaderPath)
{
	std::string shaderCode;
	std::ifstream shaderFile;
	std::stringstream shaderStream;

	// Ensure ifstream objects can throw exceptions
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// Open files
		shaderFile.open(shaderPath);

		// Read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();

		// Close file handlers
		shaderFile.close();

		// Convert stream into string
		shaderCode = shaderStream.str();

	}
	catch (const std::exception&)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << '\n';
	}

	return shaderCode;
}

#pragma endregion
// -----------------------------------------------------------------------------

class Shader
{
private:

	// Shader program ID
	GLuint ID;

public:

	// The constructor will read and compile both shaders
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		this->ID = 0;

		// 1. Get the vertex and fragment source code from filePath and convert it to string

		std::string vertexCode = readShaderFile(vertexPath);
		std::string fragmentCode = readShaderFile(fragmentPath);

		// Convert string into char*

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// 2. Shader compilation

		// Vertex Shader object and fragment shader object creation
		GLuint vertexShader, fragmentShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		compileShader(vertexShader, vShaderCode);
		compileShader(fragmentShader, fShaderCode);

		// Shader program creation and linking
		this->ID = glCreateProgram();
		glAttachShader(this->ID, vertexShader);
		glAttachShader(this->ID, fragmentShader);
		glLinkProgram(this->ID);

		// Check linking errors
		linkShaderErrorCheck(this->ID);

		// Once the linking proccess is done shader objects are no longer needed
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	~Shader()
	{
		glUseProgram(0);
		glDeleteProgram(this->ID);
	}

	inline GLuint getID() const { return this->ID; }

	// Activate shader
	void use()
	{
		glUseProgram(this->ID);
	}

	/*	Uniform utility functions:
	*
	*	Get the uniform "location" (var it self) in the shader program to be able to edit
	*	Set the values of the uniform at the given location
	*/
	void setBoolUniform(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
	}
	void setIntUniform(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
	}
	void setUIntUniform(const std::string& name, unsigned int value) const
	{
		glUniform1ui(glGetUniformLocation(this->ID, name.c_str()), value);
	}
	void setFloatUniform(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
	}
	void setFloat3Uniform(const std::string& name, float v1, float v2, float v3) const
	{
		glUniform3f(glGetUniformLocation(this->ID, name.c_str()), v1, v2, v3);
	}
	void setFloat4Uniform(const std::string& name, float v1, float v2, float v3, float v4) const
	{
		glUniform4f(glGetUniformLocation(this->ID, name.c_str()), v1, v2, v3, v4);
	}
	void setMat3Uniform(const std::string& name, glm::mat3 value) const
	{
		glUniformMatrix3fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}
	void setMat4Uniform(const std::string& name, glm::mat4 value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

};

#endif // !SHADERS_S_H
