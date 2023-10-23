
#include <SHADER/shader_s.h>

// Utils -----------------------------------------------------------------------
#pragma region "Shader construct utility functions"

void compileShader(unsigned int shader, const char* shaderCode)
{
	int comopilationFlag;
	char infoLog[512];

	glShaderSource(shader, 1, &shaderCode, NULL); // Attach shader source code to shader object
	glCompileShader(shader); // Compile the shader source code to the shader object

	// Check if compilation of the shader was successful :: snippet (Always the same)
	glGetShaderiv(shader, GL_COMPILE_STATUS, &comopilationFlag);
	if (!comopilationFlag)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}
}

void linkShaderErrorCheck(unsigned int shaderProgram)
{
	int linkFlag;
	char infoLog[512];

	// Check linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkFlag);
	if (!linkFlag)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}
}

string readShaderFile(const char* shaderPath)
{
	string shaderCode;
	ifstream shaderFile;
	stringstream shaderStream;

	// Ensure ifstream objects can throw exceptions
	shaderFile.exceptions(ifstream::failbit | ifstream::badbit);

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
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
	}

	return shaderCode;
}

#pragma endregion
// -----------------------------------------------------------------------------

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	ID = 0;

	// 1. Get the vertex and fragment source code from filePath and convert it to string

	string vertexCode = readShaderFile(vertexPath);
	string fragmentCode = readShaderFile(fragmentPath);

	// Convert string into char*
	
	const char* vShaderCode = vertexCode.c_str(); 
	const char* fShaderCode = fragmentCode.c_str();

	// 2. Shader compilation

	// Vertex Shader object and fragment shader object creation
	unsigned int vertexShader, fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	compileShader(vertexShader, vShaderCode);
	compileShader(fragmentShader, fShaderCode);

	// Shader program creation and linking
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	// Check linking errors
	linkShaderErrorCheck(ID);

	// Once the linking proccess is done shader objects are no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// Activate shader
void Shader::use()
{
	glUseProgram(ID);
}

/*	Uniform utility functions:
* 
*	Get the uniform "location" (var it self) in the shader program to be able to edit
*	Set the values of the uniform at the given location
*/
void Shader::setBoolUniform(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setIntUniform(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setUIntUniform(const std::string& name, unsigned int value) const
{
	glUniform1ui(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloatUniform(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat3Uniform(const std::string& name, float v1, float v2, float v3) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
}
void Shader::setFloat4Uniform(const std::string& name, float v1, float v2, float v3, float v4) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3, v4);
}
