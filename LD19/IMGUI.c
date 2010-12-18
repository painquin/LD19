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

#include "IMGUI.h"


static int mousedown;
static int mousex, mousey;

static int hotitem;
static int activeitem;
static int mousedown;

GLuint button_tileset;

static float square_quad[] =
{
	0, 0,
	0, -64,
	64, -64,
	64, 0
};

static float color_idle[] =
{
	0.8f, 0.8f, 0.8f,
	0.8f, 0.8f, 0.8f,
	0.8f, 0.8f, 0.8f,
	0.8f, 0.8f, 0.8f,
};

static float color_hot[] =
{
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
};

static float color_down[] =
{
	0.9f, 0.9f, 0.9f,
	0.9f, 0.9f, 0.9f,
	0.9f, 0.9f, 0.9f,
	0.9f, 0.9f, 0.9f,
};

static float *texcoords;

void imgui_init(char *tilepath, int max_tiles, int width)
{
	GLFWimage image;
	float w,h;
	int i;
	int x, y;
	float x1, y1, x2, y2;

	if (glfwReadImage(tilepath, &image, 0) != GL_TRUE)
		return;

	texcoords = (float *)calloc(max_tiles * 8, sizeof(float));

	glGenTextures(1, &button_tileset);
	glBindTexture(GL_TEXTURE_2D, button_tileset);


	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, image.Format, image.Width, image.Height,
		0, image.Format, GL_UNSIGNED_BYTE,
		image.Data);
	
	w = (float)image.Width;
	h = (float)image.Height;

	for(i = 0; i < max_tiles; ++i)
	{
		x = (i % width) * 64;
		y = (i / width) * 64;

		x1 = x / w;
		y1 = y / h;
		x2 = (x + 64) / w;
		y2 = (y + 64) / h;
		
		texcoords[(i * 8) + 0] = x1;
		texcoords[(i * 8) + 1] = 1 - y1;

		texcoords[(i * 8) + 2] = x1;
		texcoords[(i * 8) + 3] = 1 - y2;

		texcoords[(i * 8) + 4] = x2;
		texcoords[(i * 8) + 5] = 1 - y2;

		texcoords[(i * 8) + 6] = x2;
		texcoords[(i * 8) + 7] = 1 - y1;
	}


	glfwFreeImage(&image);
}

extern int gl_Width, gl_Height;

void imgui_prepare()
{
	hotitem = 0;
	glfwGetMousePos(&mousex, &mousey);
	mousex -= gl_Width / 2;
	mousey -= gl_Height / 2;
	mousedown = glfwGetMouseButton(GLFW_MOUSE_BUTTON_1);
}

void imgui_finish()
{
	if (mousedown == 0)
	{
		activeitem = 0;
	}
	else if (activeitem == 0)
	{
		activeitem = -1;
	}
}

static int regionhit(int x1, int y1, int x2, int y2)
{
	if (mousex >= x1 && mousex < x2 &&
		mousey >= y1 && mousey < y2) {
			return 1;
	}
	return 0;
}

int imgui_button(int tile, int x, int y, int _id)
{
	glBindTexture(GL_TEXTURE_2D, button_tileset);
	
	
	
	glLoadIdentity();
	glTranslatef((float)x, (float)-y, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, square_quad);

	if (regionhit(x, y, x + 64, y + 64))
	{
		hotitem = _id;
		if (activeitem == 0 && mousedown != 0)
		{
			activeitem = _id;
		}
	}

	if (hotitem == _id)
	{
		if (activeitem == _id)
		{
			glColorPointer(3, GL_FLOAT, 0, color_down);
		}
		else
		{
			glColorPointer(3, GL_FLOAT, 0, color_hot);
		}
	}
	else
	{
		glColorPointer(3, GL_FLOAT, 0, color_idle);
	}

	glDisable(GL_TEXTURE_2D);
	glDrawArrays(GL_QUADS, 0, 4);
	glEnable(GL_TEXTURE_2D);

	glColor3f(1,1,1);
	glDisableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
	glDrawArrays(GL_QUADS, 0, 4);

	glTexCoordPointer(2, GL_FLOAT, 0, texcoords + tile * 8);
	glDrawArrays(GL_QUADS, 0, 4);

	if (mousedown == 0 &&
		hotitem == _id &&
		activeitem == _id)
		return 1;

	return 0;
}

