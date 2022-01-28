module;

#include "common.h"

export module Primitives;

export class Quad
{
public:
	static inline GLuint vaoId = -1;
	static inline GLuint vboId = -1;
	static inline GLuint iboId = -1;
	static inline GLuint shaderProgramId = -1;

	static inline std::vector<uint16_t> indexData = { 0, 1, 2, 3 };
	static inline std::vector<GLfloat> vertexData{
		-1.0f, -1.0f, 0.0f,  
		1.0f, -1.0f, 0.0f,   
		1.0f, 1.0f, 0.0f,    
	};

	static inline std::vector<GLfloat> uvData{
		0.0f, 0.0f, 
		1.0f, 0.0f,
		1.0f, 1.0f, 
		0.0f, 1.0f
	};


	static void Install()  
	{
		// Check if we've initialized already
		if (Quad::shaderProgramId > 0)
			return;

		///////////
		/// Shader. I've stuffed it in here for now, plan to separate later
		///////////

		const char* vertShaderSource = R""""(#version 330 core
layout (location = 0) in vec3 aPosition;
void main() {
	gl_Position = vec4(aPosition, 1.0);
}
		)"""";

		const char* fragShaderSource = R""""(#version 330 core
//uniform sampler2D uSampler;
out vec3 color;

void main() {
	// color = texture2D(uSampler, vUV);
	color = vec3(1, 0, 1);
}
		)"""";

		GLint compileResult = GL_FALSE;
		int infoLogLength;

		// Compile Vertex Shader
		//GLuint vertShaderProgram = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vertShaderSource); 
		GLuint vertShaderProgram = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertShaderProgram, 1, &vertShaderSource, NULL);
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
		glShaderSource(fragShaderProgram, 1, &fragShaderSource, NULL);
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
		Quad::shaderProgramId = glCreateProgram();
		glAttachShader(Quad::shaderProgramId, vertShaderProgram);
		glAttachShader(Quad::shaderProgramId, fragShaderProgram);
		glLinkProgram(Quad::shaderProgramId);

		// Check the program
		glGetProgramiv(Quad::shaderProgramId, GL_LINK_STATUS, &compileResult);
		glGetProgramiv(Quad::shaderProgramId, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			std::vector<char> shaderProgramErrorMessage(infoLogLength + 1);
			glGetProgramInfoLog(Quad::shaderProgramId, infoLogLength, NULL, &shaderProgramErrorMessage[0]);
			std::cout << "Link Shader Error: " << &shaderProgramErrorMessage[0] << std::endl;
		}

		glDeleteShader(vertShaderProgram);
		glDeleteShader(fragShaderProgram);

		///////////
		/// Quad Mesh Buffers
		///////////


		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		Quad::vaoId = vao;

		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Quad::vertexData.data()), Quad::vertexData.data(), GL_STATIC_DRAW);
		Quad::vboId = vbo;
		glUseProgram(Quad::shaderProgramId);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		/*GLuint ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Quad::indexData.data()), Quad::indexData.data(), GL_STATIC_DRAW);
		Quad::iboId = ibo;*/
	}


};