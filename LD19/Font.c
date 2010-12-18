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
#include <malloc.h>
#include <gl/GL.h>
#include <gl/glfw.h>

#include "Font.h"

font_t *font_load(const char *path, int flags, char start, char end, int width)
{
	GLFWimage image;
	font_t *font;
	float w,h;
	int num_chars, i;
	int x, y;
	float x1, y1, x2, y2;

	if (glfwReadImage(path, &image, 0) != GL_TRUE)
		return NULL;

	font = (font_t *)malloc(sizeof(font_t));

	num_chars = (end - start) + 1;


	font->start = start;
	font->end = end;
	font->width = width;
	font->texcoords = (float *)calloc(num_chars * 8, sizeof(float));

	glGenTextures(1, &font->texname);
	glBindTexture(GL_TEXTURE_2D, font->texname);


	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	font->ch_width = image.Width / width;
	font->ch_height = (image.Height * width) / (num_chars); // image.Height / (num_chars / width)
	
	glTexImage2D(GL_TEXTURE_2D, 0, image.Format, image.Width, image.Height,
		0, image.Format, GL_UNSIGNED_BYTE,
		image.Data);
	
	w = (float)image.Width;
	h = (float)image.Height;

	for(i = 0; i < num_chars; ++i)
	{
		x = (i % font->width) * font->ch_width;
		y = (i / font->width) * font->ch_height;

		x1 = x / w;
		y1 = y / h;
		x2 = (x + font->ch_width) / w;
		y2 = (y + font->ch_height) / h;
		
		font->texcoords[(i * 8) + 0] = x1;
		font->texcoords[(i * 8) + 1] = 1 - y1;

		font->texcoords[(i * 8) + 2] = x1;
		font->texcoords[(i * 8) + 3] = 1 - y2;

		font->texcoords[(i * 8) + 4] = x2;
		font->texcoords[(i * 8) + 5] = 1 - y2;

		font->texcoords[(i * 8) + 6] = x2;
		font->texcoords[(i * 8) + 7] = 1 - y1;
	}


	glfwFreeImage(&image);

	return font;

}

void font_free(font_t *font) {
	free(font->texcoords);
	glDeleteTextures(1, &font->texname);
	free(font);
}

float quad[] = {
	0, 0,
	0, -1,
	0.5, -1,
	0.5, 0
};

extern int picked;

void font_drawText(font_t *font, const char *text) {
	unsigned int i, len;

	glBindTexture(GL_TEXTURE_2D, font->texname);
	glPushMatrix();
	
	glScalef((float)font->ch_width, (float)font->ch_height, 1.0f);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, quad);
	
	glPushName(0);
	for(i = 0, len = strlen(text); i < len; ++i) {
		glLoadName(i);
		if (picked == i) {
			glColor3f(1.0f, 1.0f, 0.0f);
		} else {
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		glTranslatef(0.5f, 0, 0);
		glTexCoordPointer(2, GL_FLOAT, 0, font->texcoords + (text[i] - font->start) * 8);
		glDrawArrays(GL_QUADS, 0, 4);
	}
	

	glPopMatrix();

}