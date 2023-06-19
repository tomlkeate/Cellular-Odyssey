#pragma once
#include <curses.h>
#include <stdlib.h>

// Configure file for Cellular Odyssey
// YOU MUST CLEAN UP THE FILE AND RECOMPILE FOR THE CHANGES TO TAKE EFFECT
// make clean
// make run

// change these macros to change the colors if your terminal supports colors
#define MAIN_BKG_COLOR /* must be 8 */ 8, 0, 0, 0 // color number (do not change) red green blue
#define TITLE_COLOR /* must be 9 */ 9, 255, 165, 0
#define FIELD_BKG_COLOR /* must be 10 */ 10, 0, 0, 0
#define FIELD_COLOR /* must be 11 */ 11, 120, 120, 255

// used to convert 0-255 rgb values to 0-1000
#define CONVERT_FROM_0_255_TO_0_1000(arg1) \
 (int)( ((double)arg1 / 255.0) * 1000.0) 

// changing this character will change what a cell looks like
#define CELL '~'

// set the exit message
#define EXIT_MESSAGE "GOODBYE!!"

// set the speed of the cell updates (default 100) larger is slower -1 is don't update (+ and - in playing mode will change this value)
#define GAME_SPEED 100

// turn on exit bell
#define EXIT_BELL TRUE
