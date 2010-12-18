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



tech_t TechTree[] =
{
	{ "NONE", "Untech", Tech_None, 0, 0 },
	{ "Fire",    "Fire is the key to advancement. Get those monkeys moving!", Tech_Fire, 0, 0 },
	{ "Mining",  "Cut up rocks.", Tech_Mining, 1, 0 },
	{ "Hunting", "Cut up animals.", Tech_Hunting, 1, 0 },
	{ "Farming", "Cut up plants.", Tech_Farming, 1, 0 }
};

void init_single_tech(int id)
{
	TechTree[id].prereqs = (int *)calloc(TechTree[id].prereq_count, sizeof(int));
}

void initialize_techs()
{
	int i;
	for(i = Tech_Fire; i < Tech_MAX; ++i) {
		init_single_tech(i);
	}

	TechTree[Tech_Mining].prereqs[0] = Tech_Fire;
	TechTree[Tech_Hunting].prereqs[0] = Tech_Fire;
	TechTree[Tech_Farming].prereqs[0] = Tech_Fire;
}

