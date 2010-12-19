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

#include "LD19.h"
#include "Tech.h"

tech_t *TechTree = NULL;

static tech_id_t last_id;
static size_t idx;

void tech_create(tech_id_t id, const char *name, const char *desc, size_t prereq_count, int cost, int Order, int Peace, int Change, int Wealth, int Growth)
{
	TechTree[id].name = name;
	TechTree[id].description = desc;
	TechTree[id].prereq_count = prereq_count;
	TechTree[id].discovered = 0;
	TechTree[id].prereqs = (int *)calloc(TechTree[id].prereq_count, sizeof(int));
	
	TechTree[id].cost = cost;

	TechTree[id].Order = Order;
	TechTree[id].Peace = Peace;
	TechTree[id].Change = Change;
	TechTree[id].Wealth = Wealth;
	TechTree[id].Growth = Growth;

	last_id = id;
	idx = 0;
}

void tech_add_prereq(tech_id_t prereq)
{
	TechTree[last_id].prereqs[idx++] = prereq;
}

void tech_init()
{
	TechTree = (tech_t *)calloc(Tech_MAX, sizeof(tech_t));

	tech_create(Tech_Fire, "Fire", "Fire is the key to advancement. Get those monkeys moving!", 0, 0, 0, 0, 5, 0, 5);
	tech_create(Tech_Mining, "Mining", "Cut up rocks.", 1, 10, 5, 3, 5, 5, 0);
	tech_create(Tech_Hunting, "Hunting", "Cut up animals.", 1, 10, 3, -4, 3, 1, 1);
	tech_create(Tech_Farming, "Farming", "Cut up plants.", 1, 10, 8, 1, 4, 2, 3);

	tech_add_prereq(Tech_Fire);
	tech_add_prereq(Tech_Fire);
	tech_add_prereq( Tech_Fire);

	tech_create(Tech_AnimalHusbandry, "Animal Husbandry", "Keeping your animals healthy.", 2, 15, 10, 5, 3, 5, 3);
	tech_add_prereq(Tech_Hunting);
	tech_add_prereq(Tech_Farming);

	tech_create(Tech_Mathematics, "Mathematics", "I'm a mathemagician!", 3, 25, 10, 0, 10, 0, 1);
	tech_add_prereq(Tech_Hunting);
	tech_add_prereq(Tech_Farming);
	tech_add_prereq(Tech_Mining);

	tech_create(Tech_Metalworking, "Metalworking", "Turning rocks into shinies.", 1, 15, 3, -10, 5, 3, 0);
	tech_add_prereq(Tech_Mining);
	
	tech_create(Tech_Writing, "Writing", "Words and letters.", 1, 15, 5, 5, 5, 0, 0);
	tech_add_prereq(Tech_Farming);

	tech_create(Tech_Medicine, "Medicine", "The healing arts.", 1, 15, 10, 10, 5, 2, 4);
	tech_add_prereq(Tech_Farming);

	tech_create(Tech_Pottery, "Pottery", "Storage, cookware and art - what's not to love?", 1, 5, 3, 0, 2, 4, 4);
	tech_add_prereq(Tech_Farming);

	tech_create(Tech_Irrigation, "Irrigation", "More water means more food!", 1, 20, 2, 0, 2, 1, 6);
	tech_add_prereq(Tech_Farming);

	tech_create(Tech_Glassmaking, "Glassmaking", "Glass is highly versatile.", 1, 20, 0, 0, 3, 4, 2);
	tech_add_prereq(Tech_Mining);

	tech_create(Tech_Literature, "Literature", "From classical poems to novels.", 1, 30, 5, 5, 10, 10, 0);
	tech_add_prereq(Tech_Writing);

	tech_create(Tech_Music, "Music", "Music is awesome.", 1, 30, 0, 5, 10, 0, 0);
	tech_add_prereq(Tech_Writing);

	tech_create(Tech_Biology, "Biology", "The science of life.", 1, 50, 5, 0, 5, 0, 10);
	tech_add_prereq(Tech_Medicine);

	tech_create(Tech_Paper, "Paper", "Wood fibers pressed flat.", 1, 40, 0, 0, 10, 5, 0);
	tech_add_prereq(Tech_Farming);

	tech_create(Tech_Sailing, "Sailing", "", 1, 40, 0, -5, 10, 15, 10);
	tech_add_prereq(Tech_Pottery);

	tech_create(Tech_Ecology, "Ecology", "", 1, 60, 20, 10, 15, -10, 5);
	tech_add_prereq(Tech_Biology);

	tech_create(Tech_Philosophy, "Philosophy", "", 1, 50, 30, 5, 20, 0, 0);
	tech_add_prereq(Tech_Writing);

	tech_create(Tech_Chemistry, "Chemistry", "", 2, 70, 20, -40, 30, 15, 30);
	tech_add_prereq(Tech_Mathematics);
	tech_add_prereq(Tech_Medicine);

	tech_create(Tech_Education, "Education", "", 2, 60, 10, 5, 30, 0, 0);
	tech_add_prereq(Tech_Mathematics);
	tech_add_prereq(Tech_Literature);

	tech_create(Tech_Corporations, "Corporations", "", 2, 60, 20, -10, -10, 10, 0);
	tech_add_prereq(Tech_Currency);
	tech_add_prereq(Tech_Paper);

	tech_create(Tech_Gunpowder, "Gunpowder", "", 1, 100, -10, -40, 20, 20, -5);
	tech_add_prereq(Tech_Chemistry);

	tech_create(Tech_OilRefining, "Oil Refining", "", 2, 100, -10, -20, 10, 40, 10);
	tech_add_prereq(Tech_Chemistry);
	tech_add_prereq(Tech_Metalworking);

	tech_create(Tech_Electricity, "Electricity", "", 1, 100, 0, -10, 40, 40, 20);
	tech_add_prereq(Tech_Metalworking);

	tech_create(Tech_Physics, "Physics", "", 1, 80, 5, 0, 40, 0, 0);
	tech_add_prereq(Tech_Mathematics);

	tech_create(Tech_Currency, "Currency", "", 2, 60, 10, 10, 20, 50, 10);
	tech_add_prereq(Tech_Mathematics);
	tech_add_prereq(Tech_Metalworking);

	tech_create(Tech_Astronomy, "Astronomy", "", 1, 80, 20, 10, 40, 0, 0);
	tech_add_prereq(Tech_Glassmaking);

	tech_create(Tech_Mechanics, "Mechanics", "", 1, 80, 10, -30, 10, 10, 5);
	tech_add_prereq(Tech_Metalworking);

	tech_create(Tech_Banking, "Banking", "", 1, 70, 30, 0, 10, 30, 0);
	tech_add_prereq(Tech_Currency);

	tech_create(Tech_Lasers, "Lasers", "", 2, 120, 0, -30, 10, 5, 0);
	tech_add_prereq(Tech_Electricity);
	tech_add_prereq(Tech_Physics);

	tech_create(Tech_AtomicPower, "Atomic Power", "", 2, 200, 30, -70, 80, 80, 50);
	tech_add_prereq(Tech_Electricity);
	tech_add_prereq(Tech_Physics);

	tech_create(Tech_Combustion, "Combustion", "", 2, 100, 0, -20, 40, 30, 20);
	tech_add_prereq(Tech_SteamPower);
	tech_add_prereq(Tech_OilRefining);

	tech_create(Tech_Rocketry, "Rocketry", "", 1, 110, 0, -40, 20, 10, 0);
	tech_add_prereq(Tech_Combustion);

	tech_create(Tech_Plastics, "Plastics", "", 2, 120, 10, -10, 40, 30, 40);
	tech_add_prereq(Tech_OilRefining);
	tech_add_prereq(Tech_Engineering);

	tech_create(Tech_Engineering, "Engineering", "", 2, 60, 20, 10, 10, 15, 20);
	tech_add_prereq(Tech_Physics);
	tech_add_prereq(Tech_Mechanics);

	tech_create(Tech_SteamPower, "Steam Power", "", 1, 80, 20, -10, 40, 20, 40);
	tech_add_prereq(Tech_Mechanics);

	tech_create(Tech_AssemblyLine, "Assembly Line", "", 1, 70, 10, -10, 10, 30, 40);
	tech_add_prereq(Tech_Mechanics);

	tech_create(Tech_PrintingPress, "Printing Press", "", 2, 90, -40, 20, 50, 20, 10);
	tech_add_prereq(Tech_Mechanics);
	tech_add_prereq(Tech_Paper);

	tech_create(Tech_Navigation, "Navigation", "", 2, 80, 20, 0, 10, 30, 40);
	tech_add_prereq(Tech_Astronomy);
	tech_add_prereq(Tech_Sailing);

	tech_create(Tech_Electronics, "Electronics", "", 2, 120, -20, -20, 30, 50, 20);
	tech_add_prereq(Tech_Electricity);
	tech_add_prereq(Tech_Engineering);

	tech_create(Tech_Computers, "Computers", "", 2, 150, -30, -10, 60, 60, 10);
	tech_add_prereq(Tech_Electronics);
	tech_add_prereq(Tech_AssemblyLine);

	tech_create(Tech_Robotics, "Robotics", "", 1, 200, 0, -20, 30, 30, 10);
	tech_add_prereq(Tech_Computers);

	tech_create(Tech_Flight, "Flight", "", 2, 100, 0, -20, 30, 50, 30);
	tech_add_prereq(Tech_Engineering);
	tech_add_prereq(Tech_Physics);

	tech_create(Tech_Telecommunications, "Telecommunications", "", 2, 120, -10, 0, 50, 40, 10);
	tech_add_prereq(Tech_Electricity);
	tech_add_prereq(Tech_Engineering);

	tech_create(Tech_Economics, "Economics", "", 1, 90, 20, 0, 0, 40, 10);
	tech_add_prereq(Tech_Banking);

	tech_create(Tech_InterstellarFlight, "Interstellar Flight", "", 9, 300, 0, 0, 0, 0, 0);
	tech_add_prereq(Tech_Rocketry);
	tech_add_prereq(Tech_Plastics);
	tech_add_prereq(Tech_AtomicPower);
	tech_add_prereq(Tech_Telecommunications);
	tech_add_prereq(Tech_Flight);
	tech_add_prereq(Tech_Robotics);
	tech_add_prereq(Tech_Corporations);
	tech_add_prereq(Tech_Economics);
	tech_add_prereq(Tech_Navigation);
}

