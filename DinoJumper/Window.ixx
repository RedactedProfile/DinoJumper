module;

#include "common.h"

export module Window;

import Utility;
import Game;
import Primitives;

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cout << "[OpenGL Error](" << type << ") " << message << std::endl;
}

export class Window
{
public:
	static inline const int16_t WIN_WIDTH = 640;
	static inline const int16_t WIN_HEIGHT = 480;

	static inline bool open = true;
	static inline SDL_Window* window;
	static inline SDL_GLContext context;

	static inline Game* game;

	static void CreateWindow()
	{
		SDL_Init(SDL_INIT_EVERYTHING);


		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_NORMALIZE);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		auto newWindow = SDL_CreateWindow("SuprSecrt", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Window::WIN_WIDTH, Window::WIN_HEIGHT, SDL_WINDOW_OPENGL);

		Window::window = newWindow;
		Window::context = SDL_GL_CreateContext(Window::window);

		SDL_GL_MakeCurrent(Window::window, Window::context);
		std::cout << "GLVERSION: " << glGetString(GL_VERSION) << std::endl;

		//Initialize GLEW
		glewExperimental = GL_TRUE;
		GLenum glewError = glewInit();
		if (glewError != GLEW_OK)
		{
			printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		}

		// Enable debug output
		if (glDebugMessageCallback)
		{
			std::cout << "Registering OpenGL Debug callback" << std::endl;
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(MessageCallback, nullptr);
			GLuint unusedIds = 0;
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
		}
		else
		{
			std::cout << "glDebugMessageCallback not available" << std::endl;
		}

		

		//Use Vsync
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		}

		// Initalize OpenGL
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//glDisable(GL_CULL_FACE);
		//glDisable(GL_LIGHTING);
		

		/*glMatrixMode(GL_PROJECTION_MATRIX);
		glLoadIdentity();*/

		glViewport(0, 0, Window::WIN_WIDTH, Window::WIN_HEIGHT);
		
		// Install Primitives
		Quad::Install();


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

			Window::Update();
			Window::Display();
		}

		Window::Clean();
	}

	static void Clean()
	{
		SDL_DestroyWindow(Window::window);
	}

	static void Update()
	{
		if (Window::game == nullptr) return;
		Window::game->Update();
	}

	static void Display()
	{
		glFlush();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		
		glClear(GL_COLOR_BUFFER_BIT);

		if (Window::game != nullptr) {
			Window::game->Render();
		}


		
		SDL_GL_SwapWindow(Window::window);
	}
};