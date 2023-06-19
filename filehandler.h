#pragma once
#include "field.h"
#include "config.h"
// this will be able to load and export boards as files

int write_to_file(WINDOW *mainwin, char *file_name, char *title, const playfield field);
int read_from_file(WINDOW *mainwin, char *file_name, playfield *field);
