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
		//GLuint vertShaderProgram = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vertShaderSource); 
		GLuint vertShaderProgram = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertShaderProgram, 1, &vertCode, NULL);
		glCompileShader(vertShaderProgram);
		// Check Vertex Shader
		glGetShaderiv(vertShaderProgram, GL_COMPILE_STATUS, &compileResult);
		glGetShaderiv(vertShaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			std::vector<char> vertShaderErrorMessage(infoLogLength + 1);
			glGetShaderInfoLog(vertShaderProgram, infoLogLength, NULL, &vertShaderErrorMessage[0]);
			std::cout << "Vert Shader Error: " << &vertShaderErrorMessage[0] << std::endl;
		}

		// Compile Frag Shader
		//GLuint fragShaderProgram = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fragShaderSource);
		GLuint fragShaderProgram = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShaderProgram, 1, &fragCode, NULL);
		glCompileShader(fragShaderProgram);
		// Check Frag Shader
		glGetShaderiv(fragShaderProgram, GL_COMPILE_STATUS, &compileResult);
		glGetShaderiv(fragShaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			std::vector<char> fragShaderErrorMessage(infoLogLength + 1);
			glGetShaderInfoLog(fragShaderProgram, infoLogLength, NULL, &fragShaderErrorMessage[0]);
			std::cout << "Frag Shader Error: " << &fragShaderErrorMessage[0] << std::endl;
		}

		// Link Shader Program
		shaderProgramId = glCreateProgram();
		glAttachShader(shaderProgramId, vertShaderProgram);
		glAttachShader(shaderProgramId, fragShaderProgram);
		glLinkProgram(shaderProgramId);

		// Check the program
		glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &compileResult);
		glGetProgramiv(shaderProgramId, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			std::vector<char> shaderProgramErrorMessage(infoLogLength + 1);
			glGetProgramInfoLog(shaderProgramId, infoLogLength, NULL, &shaderProgramErrorMessage[0]);
			std::cout << "Link Shader Error: " << &shaderProgramErrorMessage[0] << std::endl;
		}

		glDeleteShader(vertShaderProgram);
		glDeleteShader(fragShaderProgram);

		return shaderProgramId;
	};
};
