// LD19.cpp : Defines the entry point for the console application.
//
#include <Windows.h>

#include <gl/GL.h>
#include <gl/glfw.h>
#include <gl/glfw.h>

#include "Tech.h"

int main(int argc, char* argv[])
{
	int running = 1;

	if (!glfwInit()) {
		return -1;
	}

	if (!glfwOpenWindow(800, 600, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
		glfwTerminate();
		return -1;
	}


	while(running)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers();

		running = !glfwGetKey( GLFW_KEY_ESC) &&
			glfwGetWindowParam(GLFW_OPENED );
	}

	glfwTerminate();

	return 0;
}

