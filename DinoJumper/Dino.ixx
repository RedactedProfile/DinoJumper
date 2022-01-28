module;

#include "common.h"


export module Dino;

import Primitives;

export class Dino
{
private:
	glm::mat4x4 translation = glm::mat4(1.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 1.0f, 0.0);
	glm::mat4x4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));

public:

	void Init()
	{
		// load texture

	}

	void Update()
	{
		
	}

	void Draw()
	{
		/*glUseProgram(Quad::shaderProgramId);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, Quad::vboId);
		glVertexAttribPointer(
			0, 
			3,
			GL_FLOAT, 
			GL_FALSE, 
			0, 
			(void*)0
		);*/
		//glEnableVertexAttribArray(1);

		glUseProgram(Quad::shaderProgramId);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		
	}
};