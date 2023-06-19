#include "game.h"
#include "config.h"
#include "field.h"

int main(void) {

  // initialize the curses library
  initscr();
  cbreak();
  noecho();

  // check if terminal is large enough
  if (LINES < 15 || COLS < 85) {
    printf("Terminal window is too small exiting.\nMust be >15 x > 85");
    endwin();
    return -1;
  }

  // creates the playing field
  playfield field;
  field.rows = LINES - 1 - 3; // lines - the bottom and - the top
  field.cols = COLS - 1 - 1; // COLS = the right - the border right and left
  field.size = field.rows * field.cols;

  char *ptr = calloc(field.size + 2, sizeof(char));
  ptr[field.size + 1] = '\0';
  field.board = ptr;

  // initialize random
  srand((unsigned) time(NULL));

  // create window
  WINDOW *mainwin = newwin(LINES, COLS, 0, 0);

  // create subwin where field will be
  //WINDOW *fieldwin = subwin(mainwin, field.rows, field.cols, 3, 1);
  WINDOW *fieldwin = subwin(mainwin, field.rows, field.cols, 3, 1);

  // check if subwindow is created
  if (fieldwin == NULL) {
    printf("SUB WINDOW IS LARGER THAN ORIGINAL WINDOW %s:%d\n", __FILE__, __LINE__);
    endwin();
    delwin(mainwin);
    return -1;
  }

  // check if color enabled and then enable colors
  if (can_change_color() && has_colors()) {
    start_color();
    
    short mainbackgroundcolor[] = { MAIN_BKG_COLOR };
    init_color(mainbackgroundcolor[0], 
               CONVERT_FROM_0_255_TO_0_1000(mainbackgroundcolor[1]),
               CONVERT_FROM_0_255_TO_0_1000(mainbackgroundcolor[2]), 
               CONVERT_FROM_0_255_TO_0_1000(mainbackgroundcolor[3]) );

    short titlecolor[] = { TITLE_COLOR };
    init_color(titlecolor[0], 
               CONVERT_FROM_0_255_TO_0_1000(titlecolor[1]),
               CONVERT_FROM_0_255_TO_0_1000(titlecolor[2]), 
               CONVERT_FROM_0_255_TO_0_1000(titlecolor[3]) );

    init_pair(1, titlecolor[0], mainbackgroundcolor[0]);

    wattron(mainwin, COLOR_PAIR(1));

    short fieldbackgroundcolor[] = { FIELD_BKG_COLOR };
    init_color(fieldbackgroundcolor[0],
               CONVERT_FROM_0_255_TO_0_1000(fieldbackgroundcolor[1]), 
               CONVERT_FROM_0_255_TO_0_1000(fieldbackgroundcolor[2]), 
               CONVERT_FROM_0_255_TO_0_1000(fieldbackgroundcolor[3]) );

    short fieldcolor[] = { FIELD_COLOR };
    init_color(fieldcolor[0], 
               CONVERT_FROM_0_255_TO_0_1000(fieldcolor[1]), 
               CONVERT_FROM_0_255_TO_0_1000(fieldcolor[2]),
               CONVERT_FROM_0_255_TO_0_1000(fieldcolor[3]) );

    init_pair(2, fieldcolor[0], fieldbackgroundcolor[0]);

    wattron(fieldwin, COLOR_PAIR(2));

    // update display
    touchwin(mainwin);
    wrefresh(mainwin);
  }

  // don't move my cursor after I refresh the screen
  leaveok(mainwin, TRUE);

  // allow keypad
  keypad(fieldwin, TRUE);

  // start game loop
  game_loop(mainwin, fieldwin, &field);

  // clears up the board after making sure it isn't already free
  if (field.board != NULL) {
    free(field.board);
    field.board = NULL;
  } else if (ptr != NULL) {
    free(ptr);
    ptr = NULL;
  }

  // delete window and end it
  endwin();
  delwin(fieldwin);
  delwin(mainwin);
  exit_curses(0);

  return 0;
}
