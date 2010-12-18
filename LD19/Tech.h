/*
	TODO: license
*/
#pragma once

typedef struct tech_s tech_t;

struct tech_s {
	char *name;
	char *description;

	char *graphic;

	int prereq_count;
	tech_t *prereqs;
};

