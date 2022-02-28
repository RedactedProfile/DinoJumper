module;

#include "common.h"

export module Primitives;

import Shader;

export class Quad
{
public:
	static inline GLuint vaoId = 0;
	static inline GLuint vboId = 0;
	static inline GLuint iboId = 0;
	static inline GLuint shaderProgramId = 0;
	static inline bool installed = false;

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	
	static inline std::vector<unsigned int> indexData = { 
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	static inline std::vector<float> vertexData{
		0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
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

		const char* vertShaderSource = R""""(#version 460 core
layout (location = 0) in vec3 aPosition;
void main() {
	gl_Position = vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
}
		)"""";

		const char* fragShaderSource = R""""(#version 460 core
//uniform sampler2D uSampler;
out vec4 FragColor;

void main() {
	// color = texture2D(uSampler, vUV);
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
		)"""";

		Quad::shaderProgramId = Shader::Compile(vertShaderSource, fragShaderSource);

		///////////
		/// Quad Mesh Buffers
		///////////

		unsigned int VBO, VAO, EBO;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 4 * Quad::vertexData.size(), Quad::vertexData.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * Quad::indexData.size(), Quad::indexData.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		Quad::vaoId = VAO;
		Quad::iboId = EBO;
		Quad::vboId = VBO;

		Quad::installed = true;
	}
};