#pragma once
#include "field.h"
#include "config.h"
// this will be able to load and export boards as files

int write_to_file(char * filename, char * title,const playfield field);
int read_from_file(WINDOW *mainwin, WINDOW *fieldwin, char *filename, playfield *field);
