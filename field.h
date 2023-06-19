#pragma once
#include <time.h>
#include "config.h"

typedef struct __PLAYFiELD__ {
  char *board;
  int size;
  int rows;
  int cols;
} playfield;

int field_index(int y, int x, const playfield field);
int copy_field(playfield *field1, const playfield field2);
playfield create_new_field_from_field(const playfield field);
int free_field(playfield *newfield);
int wait(int ms); 
void fill_field(playfield *field);
void clear_field(playfield *field);
void insert_char(int y_pos, int x_pos, char c, playfield *field);
void delete_char(int y_pos, int x_pos, playfield *field);
