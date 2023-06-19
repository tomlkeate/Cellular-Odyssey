#pragma once
#include "config.h"
#include "field.h"

void draw_field(WINDOW *mainwin, WINDOW *fieldwin, const playfield field);
void draw_title(WINDOW *mainwin);
void draw_game_title(WINDOW *mainwin, char * title);

void resize_display(WINDOW *mainwin, WINDOW *fieldwin, int * playing, playfield *field);

void draw_quit(WINDOW *mainwin);

void draw_cell(int y_pos, int x_pos, playfield *field);
void draw_point_update_win(WINDOW *mainwin, WINDOW *fieldwin, int y_pos, int x_pos, char c, playfield *field);
void delete_char_update_win(WINDOW *mainwin, WINDOW *fieldwin, int y_pos, int x_pos, playfield *field);

void clear_messages(WINDOW *mainwin);
void clear_title(WINDOW *mainwin);

int get_int_input(WINDOW *mainwin, char *prompt);
char *get_string_input(WINDOW *mainwin, char *prompt);

void print_message(WINDOW *mainwin, char *prompt);

int write_to_file_display(WINDOW *mainwin, WINDOW *fieldwin, const playfield field);
int read_from_file_display(WINDOW *mainwin, WINDOW *fieldwin, playfield *field);
