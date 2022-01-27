module;

#include "common.h"


export module Dino;

import Primitives;


export class Dino
{
private:
	Quad renderable;

public:

	void Init()
	{
		renderable = Quad::Create();
	}
};