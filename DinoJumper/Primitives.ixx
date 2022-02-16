module;

#include "common.h"

export module Primitives;

import Shader;

export class Quad
{
public:
	static inline GLuint vaoId = -1;
	static inline GLuint vboId = -1;
	static inline GLuint iboId = -1;
	static inline GLuint shaderProgramId = -1;
	static inline bool installed = false;

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
		if (Quad::installed)
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

		Quad::shaderProgramId = Shader::Compile(vertShaderSource, fragShaderSource);

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

		GLuint ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Quad::indexData.data()), Quad::indexData.data(), GL_STATIC_DRAW);
		Quad::iboId = ibo;

		Quad::installed = true;
	}


};