#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

GLuint CompilerShader(GLuint type, const std::string& source)
{
	GLuint id = glCreateShader(type);
	const char* src = &source[0];
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//some error handling
	int success;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "Shader compilation failed: " << infoLog << std::endl;
	}
	
	return id;
}

GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLuint program = glCreateProgram();
	GLuint vs = CompilerShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fs = CompilerShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}