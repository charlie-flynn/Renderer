#include "Context.h"
#include <iostream>

#include <GL/glew.h> // Include GLEW before GLFW or else a scary bug will show up in your room
#include <GLFW/glfw3.h>

void APIENTRY errorCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message,
	const void* userParam)
{
	std::cerr << message << std::endl;
}

bool aie::Context::Init(int Width, int Height, const char* Title)
{
	// GLFW
	glfwInit();
	Window = glfwCreateWindow(640, 480, "my awesome window <3", nullptr, nullptr);
	glfwMakeContextCurrent(Window);

	// GLEW
	glewInit();

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// init glew
	glewInit();

	// debug stuff
#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(errorCallback, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, true);
#endif

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glClearColor(255, 0, 255, 1);

    return true;
}

void aie::Context::Tick()
{
	glfwPollEvents();
	glfwSwapBuffers(Window);
}

void aie::Context::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void aie::Context::Terminate()
{
	glfwDestroyWindow(Window);

	glfwTerminate();
}

bool aie::Context::ShouldClose()
{
	return glfwWindowShouldClose(Window);
}

