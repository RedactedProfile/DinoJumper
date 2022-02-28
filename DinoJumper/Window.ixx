module;

#include "common.h"

export module Window;

import Utility;
import Shader;
import Game;
import Primitives;

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cout << "[OpenGL Error](" << type << ") " << message << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

export class Window
{
public:
	static inline const int16_t WIN_WIDTH = 800;
	static inline const int16_t WIN_HEIGHT = 300;

	static inline bool open = true;
	/*static inline SDL_Window* window;
	static inline SDL_GLContext context;*/

	static inline GLFWwindow* window;
	

	static inline GLuint frameBuffer = 0;
	static inline GLuint renderBufferObject = 0;
	static inline GLuint renderFrameBuffer = 0;
	static inline GLuint renderBufferTexture = 0;
	static inline GLuint renderScreenShader = 0;
	static inline GLuint renderVao = 0;

	static inline Game* game;

	

	static void CreateWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		Window::window = glfwCreateWindow(Window::WIN_WIDTH, Window::WIN_HEIGHT, "SuprSecrt", NULL, NULL);
		if (window == NULL) {
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return ;
		}

		glfwMakeContextCurrent(Window::window);
		glfwSetFramebufferSizeCallback(Window::window, framebuffer_size_callback);

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

		
		// Initalize OpenGL
		glClearColor(0.39f, 0.58f, 0.93f, 1.0f);


		glViewport(0, 0, Window::WIN_WIDTH, Window::WIN_HEIGHT);
		
		// Install Primitives
		Quad::Install();


		Window::game = new Game();

		// quick hack, this should be initialized elsewhere
		Window::game->NewGame();
	}

	static void Run()
	{
		while (!glfwWindowShouldClose(Window::window))
		{
			Window::Update();
			Window::Display();

		}

		Window::Clean();
	}

	static void Clean()
	{
		glfwTerminate();
	}

	static void Update()
	{
		if (Window::game == nullptr) return;
		Window::game->Update();
	}

	static void Display()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (Window::game != nullptr) {
			Window::game->Render();
		}

		// Render ImGui Stuff

		// Swap
		glfwSwapBuffers(Window::window);
		glfwPollEvents();
	}
};