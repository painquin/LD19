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
#include <math.h>
#include <stdio.h>


#include "LD19.h"
#include "Game.h"
#include "Tech.h"
#include "Font.h"

#include "IMGUI.h"

game_t game;


font_t *font12, *font16, *font22, *font54;

int gl_Width, gl_Height;

void GLResize(int w, int h) {

	if (h == 0) h = 1;

	gl_Width = w;
	gl_Height = h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(-w / 2, w / 2, -h / 2, h / 2);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int picked = -1;

void DrawMenu()
{
	font_drawTextc(font54, "City State: Inspired Destiny", -50, -280);
	if (imgui_text(font22, "Begin", -200, -200, GEN_ID))
	{
		tech_init();
		game.state = GS_PLAYING;
	}

	if (imgui_text(font22, "About", -200, -140, GEN_ID))
	{
		game.state = GS_ABOUT;
	}

	if (imgui_text(font22, "Exit", -200, -80, GEN_ID))
	{
		glfwTerminate();
		exit(0);
	}
}

float *skyV = NULL, *skyC = NULL;

const float cv = 3.14f / 180.0f;
double nextThink = 0.0;
int score = 0;
void UpdateGame() {
	score += 1;

	nextThink += 5.0;
}

void DrawGame()
{
	/* background bit */
	if (skyV == NULL)
	{
		int i;
		int idxV = 0, idxC = 0;

		skyV = (float *)calloc(12 * 2 * 3, sizeof(float));
		skyC = (float *)calloc(12 * 3 * 3, sizeof(float));

		for(i = 8; i < 180; i += 15)
		{
			/* Add center vertex */
			skyV[idxV++] = 0.0f;
			skyV[idxV++] = 0.0f;
			/* Add center vertex color */
			skyC[idxC++] = 1.0f;
			skyC[idxC++] = 1.0f;
			skyC[idxC++] = 1.0f;

			/* Add CW vertex */
			skyV[idxV++] = cosf((i + 7) * cv) * 500;
			skyV[idxV++] = sinf((i + 7) * cv) * 500;
			/* Add CW vertex color */
			skyC[idxC++] = 0.0f;
			skyC[idxC++] = 0.0f;
			skyC[idxC++] = 0.0f;

			/* Add CCW vertex */
			skyV[idxV++] = cosf((i - 8) * cv) * 500;
			skyV[idxV++] = sinf((i - 8) * cv) * 500;
			/* Add CCW vertex color */
			skyC[idxC++] = 0.0f;
			skyC[idxC++] = 0.0f;
			skyC[idxC++] = 0.0f;
		}
	}

	glVertexPointer(2, GL_FLOAT, 0, skyV);
	glColorPointer(3, GL_FLOAT, 0, skyC);
	glCullFace(GL_NONE);
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
	glDisableClientState(GL_COLOR_ARRAY);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	glVertex2i(-gl_Width / 2 + 10, -10);
	glVertex2i(-gl_Width / 2 + 10, -gl_Height / 2 + 10);
	glVertex2i(gl_Width / 2 - 10, -gl_Height / 2 + 10);
	glVertex2i(gl_Width / 2 - 10, -10);
	glVertex2i(-gl_Width / 2 + 10, -10);
	glEnd();


	/* draw the list of techs that we can currently unlock */
	{
		int i, j;
		int xpos = -gl_Width / 2 + 15;
		int ypos = 15;
		char buf[128];

		for(i = 0; i < Tech_MAX; ++i)
		{
			if (TechTree[i].discovered) continue;

			for(j = 0; j < TechTree[i].prereq_count; ++j) {
				if (!TechTree[TechTree[i].prereqs[j]].discovered)
				{
					break;
				}
			}

			/* only if the loop completed w/o break */
			if (j == TechTree[i].prereq_count) {
				if (imgui_button(TECH_OFFSET + i, xpos, ypos, GEN_ID-i)) {
					TechTree[i].discovered = 1;
					break;
				}
				xpos += 70;
			}
		}
		xpos = -gl_Width / 2 + 15;
		ypos += 64;
		sprintf(buf, "Score: %i", score);
		font_drawText(font22, buf, xpos, ypos);
	}
}

static char *about_lines[] =
{
	"You are the guiding spirit behind an early tribe as",
	"the develop their land and discover new sciences.",
	"Influencing them through inspiration, you will",
	"spark their great minds to leadership, invention",
	"and discovery. As you unlock the technology tree,",
	"you will watch as your people survive, advance, and",
	"thrive. Many harsh dangers await; can you bring them",
	"to a still more glorious dawn?",
	0
};


void DrawAbout()
{
	int ypos = -280;
	char **c;
	font_drawTextc(font54, "About City State", -50, ypos);
	ypos += font54->ch_height + 10;

	for(c = about_lines; *c; ++c) {
		font_drawTextc(font22, *c, 0, ypos);
		ypos += font22->ch_height;
	}

	if(imgui_text(font22, "Back", -20, ypos, GEN_ID)) {
		game.state = GS_MENU;
	}
}


int main(int argc, char* argv[])
{
	int running = 1;
	
	game.state = GS_MENU;

	if (!glfwInit()) {
		return -1;
	}

	if (!glfwOpenWindow(960, 720, 8, 8, 8, 8, 0, 0, GLFW_WINDOW)) {
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(GLResize);
	glfwSetWindowTitle("City State: Inspired Destiny");

	font12 = font_load(FontPath "font_12.tga", 0, ' ', '~', 32);
	font16 = font_load(FontPath "font_16.tga", 0, ' ', '~', 32);
	font22 = font_load(FontPath "font_22.tga", 0, ' ', '~', 32);
	font54 = font_load(FontPath "font_54.tga", 0, ' ', '~', 32);

	imgui_init(TexturePath "tech/tech_tiles.tga", 5, 8);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	nextThink = glfwGetTime();
	while(running)
	{


		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();

		imgui_prepare();
		
		switch(game.state) {
		case GS_MENU:
			DrawMenu();
			break;
		case GS_ABOUT:
			DrawAbout();
			break;
		case GS_PLAYING:
			DrawGame();
			break;
		}

		imgui_finish();

		glfwSwapBuffers();

		if (glfwGetTime() > nextThink)
			UpdateGame();

		running = !glfwGetKey( GLFW_KEY_ESC) &&
			glfwGetWindowParam(GLFW_OPENED );
	}
	
	font_free(font16);

	glfwTerminate();

	return 0;
}

