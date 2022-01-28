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

	void Draw()
	{

	}
};