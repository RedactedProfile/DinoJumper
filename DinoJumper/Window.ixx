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
	static inline const int16_t WIN_WIDTH = 640;
	static inline const int16_t WIN_HEIGHT = 480;

	static inline bool open = true;
	/*static inline SDL_Window* window;
	static inline SDL_GLContext context;*/

	static inline GLFWwindow* window;
	

	static inline GLuint frameBuffer;
	static inline GLuint renderBufferObject;
	static inline GLuint renderFrameBuffer;
	static inline GLuint renderBufferTexture;
	static inline GLuint renderScreenShader;
	static inline GLuint renderVao;

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
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//glDisable(GL_CULL_FACE);
		//glDisable(GL_LIGHTING);
		
		/*glMatrixMode(GL_PROJECTION_MATRIX);
		glLoadIdentity();*/

		const char* screenVertexShaderSource = R""""(#version 440 core 
layout (location = 0) in vec2 aPos;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    TexCoords = aPos.xy;
}
		)"""";

		const char* screenFragShaderSource = R""""(#version 440 core 

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main() 
{
    gl_FragColor = texture(screenTexture, 0.5*(vec2(1,1) + TexCoords));
}
		)"""";

		Window::renderScreenShader = Shader::Compile(screenVertexShaderSource, screenFragShaderSource);

		// create off-screen rendering buffers
		// This is for the psx style rendering of low-rez render and upscaling to a higher resolution
		glGenFramebuffers(1, &Window::renderFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, Window::renderFrameBuffer);


		glGenTextures(1, &Window::renderBufferTexture);
		glBindTexture(GL_TEXTURE_2D, Window::renderBufferTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Window::WIN_WIDTH, Window::WIN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderBufferTexture, 0);

		glGenRenderbuffers(1, &Window::renderBufferObject);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Window::WIN_WIDTH, Window::WIN_HEIGHT);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR: FrameBuffer is not complete!";
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! :: " << fboStatus << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

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

		/*SDL_Event event;
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
		}*/

		Window::Clean();
	}

	static void Clean()
	{
		//SDL_DestroyWindow(Window::window);
		glfwTerminate();
	}

	static void Update()
	{
		if (Window::game == nullptr) return;
		Window::game->Update();
	}

	static void Display()
	{
		// pass 1: bind to offscreen buffer

		glBindFramebuffer(GL_FRAMEBUFFER, Window::renderFrameBuffer);
		glViewport(0, 0, Window::WIN_WIDTH, Window::WIN_HEIGHT);
		glClearColor(0.39f, 0.58f, 0.93f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);


		if (Window::game != nullptr) {
			Window::game->Render();
		}


		// pass 2: render texture to fullscreen quad
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Window::WIN_WIDTH, Window::WIN_HEIGHT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glUseProgram(renderScreenShader);
		glBindVertexArray(renderVao);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, renderBufferTexture);
		glDrawArrays(GL_TRIANGLES, 0, 24);


		// Render ImGui Stuff


		// Swap
		glfwSwapBuffers(Window::window);
		glfwPollEvents();
	}
};