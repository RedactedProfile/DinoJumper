module;

#include "common.h"

export module Shader;

export class Shader
{
public:
	static GLuint Compile(const char* vertCode, const char* fragCode)
	{
		GLuint shaderProgramId;

		GLint compileResult = GL_FALSE;
		int infoLogLength;

		// Compile Vertex Shader
		GLuint vertShaderProgram = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertShaderProgram, 1, &vertCode, NULL);
		glCompileShader(vertShaderProgram);
	
		// Check Vertex Shader
		int success;
		char infoLog[512];
		glGetShaderiv(vertShaderProgram, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertShaderProgram, 512, NULL, infoLog);
			std::cout << "[SHADERERR][VERTEX_SHADER]: " << infoLog << std::endl;
		}

		// Compile Frag Shader
		GLuint fragShaderProgram = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShaderProgram, 1, &fragCode, NULL);
		glCompileShader(fragShaderProgram);
		// Check Frag Shader
		glGetShaderiv(fragShaderProgram, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragShaderProgram, 512, NULL, infoLog);
			std::cout << "[SHADERERR][FRAGMENT_SHADER]: " << infoLog << std::endl;
		}

		// Link Shader Program
		shaderProgramId = glCreateProgram();
		glAttachShader(shaderProgramId, vertShaderProgram);
		glAttachShader(shaderProgramId, fragShaderProgram);
		glLinkProgram(shaderProgramId);

		// Check the program
		glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgramId, 512, NULL, infoLog);
			std::cout << "[SHADERERR][LINKING]: " << infoLog << std::endl;
		}

		glDeleteShader(vertShaderProgram);
		glDeleteShader(fragShaderProgram);

		return shaderProgramId;
	};
};
