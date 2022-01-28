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
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f
	};

	static inline std::vector<GLfloat> uvData{
		0.0f, 0.0f, 
		1.0f, 0.0f,
		1.0f, 1.0f, 
		0.0f, 1.0f
	};


	static void Install()  
	{
		auto test = Quad::shaderProgramId;
		// Check if we've initialized already
		if (Quad::shaderProgramId > 0)
			return;

		///////////
		/// Quad Shader
		///////////

		const char* vertShaderSource = R""""(
#version 460 core
//uniform mat4 uViewMatrix, uProjMatrix, uWorldMatrix;
layout (location = 0) in vec3 aPosition;
//attribute vec2 aUV;

varying vec2 vUV;

void main() {
	// gl_Position = uProjMatrix * uViewMatrix * uWorldMatrix * vec4(aPosition, 1.0);
	// vUV = aUV;
    gl_Position = vec4(aPosition, 1.0);
}
		)"""";

		const char* fragShaderSource = R""""(
#version 460 core
precision highp float;
uniform sampler2D uSampler;
varying vec2 vUV;

void main() {
	// gl_FragColor = texture2D(uSampler, vUV);
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
		)"""";

		GLuint vertShaderProgram = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vertShaderSource);
		GLuint fragShaderProgram = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fragShaderSource);

		Quad::shaderProgramId = glCreateProgram();
		glAttachShader(Quad::shaderProgramId, vertShaderProgram);
		glAttachShader(Quad::shaderProgramId, fragShaderProgram);
		glLinkProgram(Quad::shaderProgramId);

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

		//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
		//glEnableVertexAttribArray(0);


		GLuint ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Quad::indexData.data()), Quad::indexData.data(), GL_STATIC_DRAW);
		Quad::iboId = ibo;
	}


};