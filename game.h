#pragma once
#include "config.h"
#include "field.h"

void game_loop(WINDOW *mainwin, WINDOW * fieldwin, playfield *field);

void generate_random_spots(int n, playfield *field); 

void update_cells(playfield *field);

int check_normal_cell(int row, int col, const playfield field);

int check_left_top_corner_cell(int row, int col, const playfield field);

int check_top_cell(int row, int col, const playfield field);

int check_right_top_corner_cell(int row, int col, const playfield field);

int check_right_cell(int row, int col,const playfield field);

int check_right_bottom_corner_cell(int row, int col, const playfield field);

int check_bottom_cell(int row, int col, const playfield field);

int check_left_bottom_corner_cell(int row, int col, const playfield field);

int check_left_cell(int row, int col, const playfield field);
