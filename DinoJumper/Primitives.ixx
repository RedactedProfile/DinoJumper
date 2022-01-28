module;

#include "common.h"

export module Primitives;

export class Quad
{
public:
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


	static void InitQuad()  
	{
		// Check if we've initialized already
		if (Quad::shaderProgramId != -1)
			return;

		///////////
		/// Quad Shader
		///////////

		const char* vertShaderSource = R""""(
#version 460 core
uniform mat4 uViewMatrix, uProjMatrix, uWorldMatrix;
attribute vec3 aPosition;
attribute vec2 aUV;

varying vec2 vUV;

void main() {
	gl_Position = uProjMatrix * uViewMatrix * uWorldMatrix * vec4(aPosition, 1.0);
	vUV = aUV;
}
		)"""";

		const char* fragShaderSource = R""""(
#version 460 core
precision highp float;
uniform sampler2D uSampler;
varying vec2 vUV;

void main() {
	gl_FragColor = texture2D(uSampler, vUV);
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

		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), Quad::vertexData.data(), GL_STATIC_DRAW);
		Quad::vboId = vbo;

		GLuint ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), Quad::indexData.data(), GL_STATIC_DRAW);
		Quad::iboId = ibo;

		///////////
		/// Quad Vertex Array Object
		///////////

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);


	}


};