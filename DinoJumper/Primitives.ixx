module;

#include "common.h"

export module Primitives;

export class Quad
{
public:
	glm::mat4x4 translation;
	glm::mat4x4 rotation;
	glm::mat4x4 scale;

	std::vector<GLfloat> vertexData{
		-0.5f, -0.5f, 
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f
	};

	std::vector<uint16_t> indexData = { 0, 1, 2, 3 };

	GLuint vboId = -1;
	GLuint iboId = -1;

	static Quad Create()
	{
		Quad newQuad;

		// Create VBO
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), newQuad.vertexData.data(), GL_STATIC_DRAW);
		newQuad.vboId = vbo;

		// Create IBO
		GLuint ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), newQuad.indexData.data(), GL_STATIC_DRAW);
		newQuad.iboId = ibo;

		return newQuad;
	}
};