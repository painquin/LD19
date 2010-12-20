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

#include "Disaster.h"
#include "Tech.h"
#include "Game.h"

static int cond_flood()
{
	/* Can't lose your fields if you haven't started farming yet. */
	if(!TechTree[Tech_Farming].discovered)
		return 0;

	/* Only if growth has been sorely neglected */
	if (game.Growth < 0)
		return 1;

	return 0;
}

static int cond_war()
{
	/* War can happen at any time, but this is the disaster of losing a war */
	/* Basically if you are completely unprepared for war. */

	if (game.Peace > 40)
		return 1;
	
	return 0;
}

static int cond_sacked()
{
	/* A wealthy but defenseless city is a juicy target */

	if (game.Wealth > 40 && game.Peace > 30)
		return 1;

	return 0;
}

static int cond_earthquake()
{
	/* A poor city won't have the infrastructure to survive an earthquake */
	if (game.Wealth < 20 && game.Population > 500)
		return 1;

	return 0;
}

static int cond_nuclear()
{
	if (TechTree[Tech_AtomicPower].discovered && game.Peace < -30)
		return 1;

	return 0;
}

static int cond_robots()
{
	/* Robots taking over the world :o */
	if(TechTree[Tech_Robotics].discovered && game.Order < 60)
		return 1;
	
	return 0;
}

disaster_t disaster_table[] =
{
	{ "Flood", "A flood has wiped out your farmland.", cond_flood },
	{ "War", "Your people have been wiped out through war.", cond_war },
	{ "Barbarians", "Your city has been sacked by barbarians,\nits people dispersed and broken-hearted.", cond_sacked },
	{ "Earthquake", "Struck by a devastating earthquake, your\npeople find they do not have the resources\nto rebuild their shattered home.", cond_earthquake },
	{ "Nuclear Apocalypse", "Rising tensions between global superpowers\nhave led to the use of your nuclear arsenal.\nThe retaliation should have come as no surprise.", cond_nuclear },
	{ "Robot Revolution", "Overworked and underpaid, robots have taken\nadvantage of the chaos in your city to usurp\ncontrol from the government.", cond_robots },
};

const int disaster_count = sizeof(disaster_table) / sizeof(disaster_t);

