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

#include "LD19.h"
#include "Tech.h"
#include "Font.h"

const char* hello = "Hello, World!";



font_t *font16 = NULL;

int gl_Width, gl_Height;
void GLResize(int w, int h) {

	if (h == 0) h = 1;

	gl_Width = w;
	gl_Height = h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluPerspective(45.0f, w / (GLfloat)h, 0.1, 100.0);
	gluOrtho2D(-w / 2, w / 2, -h / 2, h / 2);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int picked = -1;

int main(int argc, char* argv[])
{
	int running = 1, hits;
	GLuint selbuf[64];
	GLint viewport[4];
	int mouseX, mouseY;

	if (!glfwInit()) {
		return -1;
	}

	if (!glfwOpenWindow(800, 600, 8, 8, 8, 8, 0, 0, GLFW_WINDOW)) {
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(GLResize);

	font16 = font_load(FontPath "font_22.tga", 0, ' ', '~', 32);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while(running)
	{


		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();

		glSelectBuffer(64, selbuf);
		glRenderMode(GL_SELECT);
		picked = -1;
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glGetIntegerv(GL_VIEWPORT, viewport);
		glfwGetMousePos(&mouseX, &mouseY);
		gluPickMatrix(mouseX, viewport[3]-mouseY, 5, 5, viewport);
		gluOrtho2D(-gl_Width / 2, gl_Width / 2, -gl_Height / 2, gl_Height / 2);
		glMatrixMode(GL_MODELVIEW);
		glInitNames();
		font_drawText(font16, "Hello, World!");
		hits = glRenderMode(GL_RENDER);
		if (hits != 0) {
			hits = selbuf[0];
			if (hits > 0) {
				picked = selbuf[3];
			}
		}
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(-gl_Width / 2, gl_Width / 2, -gl_Height / 2, gl_Height / 2);
		glMatrixMode(GL_MODELVIEW);
		font_drawText(font16, "Hello, World!");
		

		glfwSwapBuffers();

		running = !glfwGetKey( GLFW_KEY_ESC) &&
			glfwGetWindowParam(GLFW_OPENED );
	}
	
	font_free(font16);

	glfwTerminate();

	return 0;
}

