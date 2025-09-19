#pragma once

struct GLFWwindow;



namespace aie
{
	class Context
	{
	public:
		GLFWwindow* Window;

		bool Init(int Width, int Height, const char* Title);
		void Tick();
		void Clear();
		void Terminate();
		bool ShouldClose();

	};
}

