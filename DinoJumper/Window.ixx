module;

#include "common.h"

export module Window;

import Utility;
import Game;
import Primitives;

export class Window
{
public:
	static inline bool open = true;
	static inline SDL_Window* window;
	static inline SDL_Surface* windowSurface;
	static inline SDL_Renderer* renderer;
	static inline SDL_GLContext context;

	static inline Game* game;

	static void CreateWindow()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		
		auto newWindow = SDL_CreateWindow("Dino", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);

		Window::window = newWindow;
		Window::context = SDL_GL_CreateContext(Window::window);
		Window::windowSurface = SDL_GetWindowSurface(Window::window);
		Window::renderer = SDL_CreateRenderer(Window::window, -1, SDL_RENDERER_ACCELERATED);

		//Initialize GLEW
		glewExperimental = GL_TRUE;
		GLenum glewError = glewInit();

		if (glewError != GLEW_OK)
		{
			printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		}

		//Use Vsync
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		}

		
		Quad::InitQuad();

		Window::game = new Game();

		// quick hack, this should be initialized elsewhere
		Window::game->NewGame();
	}

	static void Run()
	{
		SDL_Event event;
		while (Window::open)
		{
			if (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
				case SDL_QUIT:
					Window::open = false;
					break;
				}

				Window::game->HandleEvents(event);
			}

			Window::Display();
		}

		Window::Clean();
	}

	static void Clean()
	{
		SDL_DestroyRenderer(Window::renderer);
		SDL_DestroyWindow(Window::window);
	}

	static void Display()
	{
		SDL_RenderClear(Window::renderer);
		SDL_RenderPresent(Window::renderer);
	}
};