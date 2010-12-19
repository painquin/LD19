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
#pragma once

typedef enum {
	Tech_Fire,
	Tech_Mining,
	Tech_Hunting,
	Tech_Farming,

	Tech_AnimalHusbandry,
	Tech_Mathematics,
	Tech_Metalworking,
	Tech_Writing,

	Tech_Medicine,
	Tech_Pottery,
	Tech_Irrigation,
	Tech_Glassmaking,

	Tech_Literature,
	Tech_Music,
	Tech_Biology,
	Tech_Paper,
	Tech_Sailing,

	Tech_Ecology,
	Tech_Philosophy,
	Tech_Chemistry,
	Tech_Education,

	Tech_Corporations,
	Tech_Gunpowder,
	Tech_OilRefining,
	Tech_Electricity,

	Tech_Physics,
	Tech_Currency,
	Tech_Astronomy,
	Tech_Mechanics,

	Tech_Banking,
	Tech_Lasers,
	Tech_AtomicPower,
	Tech_Combustion,

	Tech_Rocketry,
	Tech_Plastics,
	Tech_Engineering,
	Tech_SteamPower,

	Tech_AssemblyLine,
	Tech_PrintingPress,
	Tech_Navigation,
	Tech_Electronics,

	Tech_Computers,
	Tech_Robotics,
	Tech_Flight,
	Tech_Telecommunications,
	
	Tech_Economics,
	Tech_InterstellarFlight,

	Tech_MAX
} tech_id_t;

/* where in the tileset do techs start */
#define TECH_OFFSET 1

typedef struct tech_s tech_t;

struct tech_s {
	const char *name;
	const char *description;

	int id;

	int prereq_count;
	tech_id_t *prereqs;

	int discovered;

	/* Amount of inspiration required to unlock */
	int cost;
	/* Influence on leanings */

	int Order;
	int Peace;
	int Change;
	int Wealth;
	int Growth;
};

extern tech_t *TechTree;

void tech_init();

