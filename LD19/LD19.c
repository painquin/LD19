/*
Copyright (c) 2010 Andrew Hohorst

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include <Windows.h>

#include <gl/GL.h>
#include <gl/glfw.h>
#include <gl/glfw.h>

#include "Tech.h"
#include "Font.h"

const char* hello = "Hello, World!";
const char 
font_t *font16 = NULL;

void GLResize(int w, int h) {

	if (h == 0) h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluPerspective(45.0f, w / (GLfloat)h, 0.1, 100.0);
	gluOrtho2D(-w / 2, w / 2, -h / 2, h / 2);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int picked;

int main(int argc, char* argv[])
{
	unsigned int i;
	int running = 1;

	if (!glfwInit()) {
		return -1;
	}

	if (!glfwOpenWindow(800, 600, 8, 8, 8, 8, 0, 0, GLFW_WINDOW)) {
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(GLResize);

	font16 = font_load("font_22.tga", 0, ' ', '~', 32);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	while(running)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();

		font_drawText(font16, "Hello, World!");
		

		glfwSwapBuffers();

		running = !glfwGetKey( GLFW_KEY_ESC) &&
			glfwGetWindowParam(GLFW_OPENED );
	}
	
	font_free(font16);

	glfwTerminate();

	return 0;
}

