module;

#include "common.h"

export module Game;

import Dino;

export class Game
{
private:
	
	Dino* player = nullptr;

public:

	void NewGame()
	{
		player = new Dino();
		player->Init();
	}

	void HandleEvents(SDL_Event &event)
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			InputDown(event.key);
			break;
		case SDL_KEYUP:
			InputUp(event.key);
			break;
		}
	}

	void InputDown(SDL_KeyboardEvent& event)
	{

	}

	void InputUp(SDL_KeyboardEvent& event)
	{

	}
};