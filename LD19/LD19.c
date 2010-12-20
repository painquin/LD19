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
#include <string.h>
#include <stdlib.h>
#include <time.h>


#include "LD19.h"
#include "Game.h"
#include "Tech.h"
#include "Font.h"
#include "Disaster.h"
#include "IMGUI.h"

game_t game;


font_t *font16, *font22, *font54;

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

static const char *popupTitle = "Popup";
static const char *popupText;
static game_state_t oldState;

void ShowPopup(const char *title, const char *text)
{
	popupTitle = title;
	popupText = text;
	oldState = game.state;
	game.state = GS_POPUP;
}

void DrawMenu()
{
	font_drawTextc(font54, "City State: Inspired Destiny", 0, -280);
	if (imgui_text(font22, "Begin", -200, -200, GEN_ID))
	{
		tech_init(); //= { 20, -4000, 0, 0,0, 0, 1, GS_MENU };
		
		game.Population = 20;
		game.Year = -4000;
		game.Change = 0;
		game.Peace = 0;
		game.Order = 0;
		game.Wealth = 0;
		game.Growth = 1;
		game.Inspiration = 1;
		game.CostToInspire = 10;

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

int nextDisaster = -3950;

void UpdateGame() {

	/* The game begins when fire is discovered. */
	if (TechTree[Tech_Fire].discovered) {
		if (game.Year < -1000)
		{
			game.Year += 50; // 4000BC .. 1000BC (60 turns)
		}
		else if (game.Year < 1000)
		{
			game.Year += 25; // 1000BC .. 1000AD (80 turns)
		}
		else if (game.Year < 1800)
		{
			game.Year += 10; // 1000AD .. 1800AD (80 turns)
		}
		else 
		{
			game.Year += 1; // 1800AD .. ENDGAME (200+ turns)
		}

		if (game.Year == 2100)
		{
			game.state = GS_MENU;
			ShowPopup("Destruction!",
				"An asteroid strikes the Earth, wiping out all life.\n"
				"If you only you had found a way to leave your island\n"
				"of life and spread to other worlds..."
				);
		}

		if (rand() % 100 > 15)
		{
			int disaster_id = rand() % disaster_count;
			if (disaster_table[disaster_id].condition()) {
				game.state = GS_MENU;
				ShowPopup(disaster_table[disaster_id].title,
					disaster_table[disaster_id].description);
			}
		}



		game.Population += game.Growth / 5;
		game.Inspiration += game.Change / 35 + game.Population / 100 + 1;

	}



	nextThink += 1.0;
}
GLuint city_tile;

void DrawGame()
{
	int hoverID = -1;
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

	/* draw the city */
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, city_tile);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);
	glVertex2i(-160, 0);
	glTexCoord2i(0, 1);
	glVertex2i(-160, 256);
	glTexCoord2i(1, 1);
	glVertex2i(160, 256);
	glTexCoord2i(1, 0);
	glVertex2i(160, 0);
	glEnd();


	/* draw the list of techs that we can currently unlock */
	{
		int i, j, k;
		int xpos = -gl_Width / 2 + 15;
		int ypos = 15;

		char buf[256];

		sprintf_s(buf, 256, "Inspire great minds to discover advanced technology. Inspiration: %i", game.Inspiration);
		ypos += font_drawText(font16, buf, xpos, ypos);

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
				if (imgui_button(TECH_OFFSET + i, xpos, ypos, k = (GEN_ID-i))) {
					if (game.Inspiration >= TechTree[i].cost) {
						game.Inspiration -= TechTree[i].cost;
						
						game.Order += TechTree[i].Order;
						game.Peace += TechTree[i].Peace;
						game.Growth += TechTree[i].Growth;
						game.Wealth += TechTree[i].Wealth;
						game.Change += TechTree[i].Change;

						TechTree[i].discovered = 1;
						if (i == Tech_InterstellarFlight) {
							game.state = GS_MENU;
							ShowPopup("Interstellar Flight",
								"The true story of your people may finally begin.\n\n"

								"The sky calls to us\n"
								"If we do not destroy ourselves\n"
								"We will one day venture to the stars\n\n"

								"A still more glorious dawn awaits\n"
								"Not a sunrise, but a galaxy rise\n"
								"A morning filled with 400 billion suns\n"
								"The rising of the milky way\n"
								"\"A Glorious Dawn\" - Symphony of Science\n"
								"This entry is dedicated to the memory of Dr. Carl Sagan"
								);
						}
						break;
					}
				}
				else if (imgui_hotitem() == k)
				{
					hoverID = i;
				}
				xpos += 70;
			}
		}
		xpos = -gl_Width / 2 + 15;
		ypos += 75;
		if (hoverID == -1)
		{
			buf[0] = 0;
		}
		else
		{
			sprintf_s(buf, 256, 
				"%s: %i/%i inspiration.",
				TechTree[hoverID].name,
				game.Inspiration,
				TechTree[hoverID].cost
				);
		}

		ypos += font_drawText(font16, buf, xpos, ypos);

		ypos += 20;

		sprintf_s(buf, 256, "Population: %i\nChange: %i\nPeace: %i\nOrder: %i\nWealth: %i\nGrowth: %i",
			game.Population, game.Change, game.Peace, game.Order, game.Wealth, game.Growth);
		font_drawText(font16, buf, xpos, ypos);

		xpos = 0;
		sprintf_s(buf, 256, "Inspire Peace (%i)", game.CostToInspire);
		if (imgui_text(font16, buf, xpos, ypos, GEN_ID))
		{
			if (game.CostToInspire <= game.Inspiration)
			{
				game.Peace += 10;
				game.Inspiration -= game.CostToInspire;
				game.CostToInspire *= 1.2f;
			}
		}
		ypos += font16->ch_height;

		sprintf_s(buf, 256, "Inspire Defense (%i)", game.CostToInspire);
		if (imgui_text(font16, buf, xpos, ypos, GEN_ID))
		{
			if (game.CostToInspire <= game.Inspiration)
			{
				game.Peace -= 10;
				game.Inspiration -= game.CostToInspire;
				game.CostToInspire *= 1.2f;
			}
		}
		ypos += font16->ch_height;
		sprintf_s(buf, 256, "Inspire Economy (%i)", game.CostToInspire);
		if (imgui_text(font16, buf, xpos, ypos, GEN_ID))
		{
			if (game.CostToInspire <= game.Inspiration)
			{
				game.Wealth += 10;
				game.Inspiration -= game.CostToInspire;
				game.CostToInspire *= 1.2f;
			}
		}
		ypos += font16->ch_height;
		sprintf_s(buf, 256, "Inspire Order (%i)", game.CostToInspire);
		if (imgui_text(font16, buf, xpos, ypos, GEN_ID))
		{
			if (game.CostToInspire <= game.Inspiration)
			{
				game.Order += 10;
				game.Inspiration -= game.CostToInspire;
				game.CostToInspire *= 1.2f;
			}
		}
		ypos += font16->ch_height;
		sprintf_s(buf, 256, "Inspire Growth (%i)", game.CostToInspire);
		if (imgui_text(font16, buf, xpos, ypos, GEN_ID))
		{
			if (game.CostToInspire <= game.Inspiration)
			{
				game.Growth += 10;
				game.Inspiration -= game.CostToInspire;
				game.CostToInspire *= 1.2f;
			}
		}
		ypos += font16->ch_height;

		


		if (game.Year < 0)
		{
			sprintf_s(buf, 256,  "%iBC", -game.Year);
		}
		else if (game.Year == 0)
		{
			sprintf_s(buf, 256,  "1AD");
		}
		else
		{
			sprintf_s(buf, 256,  "%iAD", game.Year);
		}

		font_drawText(font22, buf, gl_Width / 2 - 100, -gl_Height / 2 + 5);
	}
}

static const char *about_lines =
	"You are the guiding spirit behind an early tribe as\n"
	"they develop their land and discover new sciences.\n"
	"Influencing them through inspiration, you will\n"
	"spark their great minds to leadership, invention\n"
	"and discovery. As you unlock the technology tree,\n"
	"you will watch as your people survive, advance, and\n"
	"thrive. Many harsh dangers await; can you bring them\n"
	"to a still more glorious dawn?"
	;


void DrawAbout()
{
	int ypos = -290;
	ypos += font_drawTextc(font54, "About City State", 0, ypos);
	ypos += 10;

	ypos += font_drawTextc(font22, about_lines, 0, ypos);
	
	if(imgui_text(font22, "Back", -20, ypos + 20, GEN_ID)) {
		game.state = GS_MENU;
	}
}


void DrawPopup()
{
	int ypos = - gl_Height / 2 + 40;

	glLoadIdentity();
	glEnable(GL_COLOR);
	glDisable(GL_TEXTURE_2D);
	/* this is on top of the game screen */
	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2i(-gl_Width / 2 + 20, gl_Height / 2-20);
	glVertex2i(-gl_Width / 2 + 20, -gl_Height / 2 + 20);
	glVertex2i(gl_Width / 2 - 20, -gl_Height / 2 + 20);
	glVertex2i(gl_Width / 2 - 20, gl_Height / 2-20);
	glVertex2i(-gl_Width / 2 + 20, gl_Height / 2 -20);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	glVertex2i(-gl_Width / 2 + 25, gl_Height / 2-25);
	glVertex2i(-gl_Width / 2 + 25, -gl_Height / 2 + 25);
	glVertex2i(gl_Width / 2 - 25, -gl_Height / 2 + 25);
	glVertex2i(gl_Width / 2 - 25, gl_Height / 2-25);
	glVertex2i(-gl_Width / 2 + 25, gl_Height / 2-25);
	glEnd();


	ypos += font_drawTextc(font54, popupTitle, 0, ypos);

	ypos += font_drawTextc(font22, popupText, 0, ypos);

	if (imgui_text(font22, "Back", -20, ypos + 20, GEN_ID)) {
		game.state = oldState;
		nextThink = glfwGetTime() + 1.0;
	}

}

int main(int argc, char* argv[])
{
	int running = 1;
	
	game.state = GS_MENU;
	
	srand((unsigned int)time(NULL));

	if (!glfwInit()) {
		return -1;
	}

	if (!glfwOpenWindow(960, 720, 8, 8, 8, 8, 0, 0, GLFW_WINDOW)) {
		glfwTerminate();
		return -1;
	}

	glGenTextures(1, &city_tile);
	glBindTexture(GL_TEXTURE_2D, city_tile);
	glfwLoadTexture2D(TexturePath "city.tga", 0);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);



	glfwSetWindowSizeCallback(GLResize);
	glfwSetWindowTitle("City State: Inspired Destiny");

	if (!(font16 = font_load(FontPath "font_16.tga", 0, ' ', '~', 32))) {
		printf("No font16\n");
	}
	if (!(font22 = font_load(FontPath "font_22.tga", 0, ' ', '~', 32))) {
		printf("No font22\n");
	}
	if (!(font54 = font_load(FontPath "font_54.tga", 0, ' ', '~', 32))) {
		printf("No font54\n");
	}

	imgui_init(TexturePath "tech/tech_tiles.tga", Tech_MAX + 1, 8);

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
		case GS_POPUP:
			DrawGame();
			DrawPopup();
			break;
		}

		imgui_finish();

		glfwSwapBuffers();

		if (game.state == GS_PLAYING && glfwGetTime() > nextThink)
			UpdateGame();

		running = !glfwGetKey( GLFW_KEY_ESC) &&
			glfwGetWindowParam(GLFW_OPENED );
	}
	
	font_free(font16);
	font_free(font22);
	font_free(font54);

	glfwTerminate();

	return 0;
}

