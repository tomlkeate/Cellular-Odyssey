#include "draw.h"
#include "config.h"
#include <string.h>

void update_display(WINDOW *mainwin, WINDOW *fieldwin) {
  touchwin(mainwin);
  wrefresh(fieldwin);
  wrefresh(mainwin);
}

void draw_field(WINDOW *mainwin, WINDOW *fieldwin, const playfield field) {

  // find current location to restore mouse position
  int x_pos, y_pos;
  getyx(fieldwin, y_pos, x_pos);
  
  // draw the field one row at a time
  for (int row = 0; row < field.rows; row++) {
    wmove(fieldwin, row, 0);
    wclrtoeol(fieldwin);
    for (int col = 0; col < field.cols; col++) {
      char c = field.board[field_index(row, col, field)];
      if (c != 0) {
        mvwaddch(fieldwin, row, col, c);
      }
    }
  }

  // restore mouse postion
  wmove(fieldwin, y_pos, x_pos);

  // update display
  update_display(mainwin, fieldwin);
}

// accept title name and display it in middle top
void draw_title(WINDOW *mainwin) {
  box(mainwin, '|', '-');
  mvwaddstr(mainwin, 1, 2, "Welcome to conways game of life!");
  mvwhline(mainwin, 2, 1, '~', getmaxx(mainwin)-2);
  wrefresh(mainwin);
}

void draw_game_title(WINDOW *mainwin, char * title) {
  mvwaddstr(mainwin, 1, ((getmaxx(mainwin)-1) / 2) - strlen(title)/2, title);
  free(title);
  touchwin(mainwin);
  wrefresh(mainwin);
}

void resize_display(WINDOW *mainwin, WINDOW *fieldwin, int * playing, playfield *field) {
  int y_max_main, x_max_main, y_max_field, x_max_field;
  getmaxyx(mainwin, y_max_main, x_max_main);
  wclear(mainwin);

  if (y_max_main < 15 || x_max_main < 85) {
    wclear(mainwin);
    *playing = FALSE;
    printf("Terminal must be Y > 15 X > 85\n");
    return;
  }

  y_max_field = y_max_main - 3 - 1;
  x_max_field = x_max_main - 1 - 1;

  int smallestRow = field->rows,
  smallestCol = field->cols;

  if (y_max_field < field->rows) {
    smallestRow = y_max_field;
  }
  if (x_max_field < field->cols)  {
    smallestCol = x_max_field;
  }

  int size = (y_max_field) * (x_max_field);
  char *newBoardPtr = calloc(size + 2, sizeof(char));
  if (newBoardPtr == NULL) {
    printf("ERROR Allocating field: %s:%d", __FILE__, __LINE__);
    return;
  }
  newBoardPtr[size + 1] = '\0';


  for (int row = 0; row < smallestRow; row++) {
    for (int col = 0; col < smallestCol; col++) {
      if (row < field->cols && col < field->cols) {
        newBoardPtr[(row * x_max_field) + col] = field->board[field_index(row, col, *field)];
      }
    }
  }

  free_field(field);
  field->rows = y_max_field;
  field->cols = x_max_field;
  field->size = field->rows * field->cols; 
  field->board = newBoardPtr;

  //resizeterm(y_max_main, x_max_main);
  //wresize(mainwin, y_max_main, x_max_main);
  //mvwin(mainwin, 0, 0);

  wresize(fieldwin, y_max_field, x_max_field);
  wclear(fieldwin);
  wrefresh(fieldwin);
  mvwin(fieldwin, 3, 1);

  draw_title(mainwin);
  draw_field(mainwin, fieldwin, *field);
}


void draw_quit(WINDOW *mainwin) {
  curs_set(0);
  mvwaddstr(mainwin, (getmaxy(mainwin) - 1)/2, (getmaxx(mainwin) - 1)/2 - strlen(EXIT_MESSAGE), EXIT_MESSAGE);
  wrefresh(mainwin);
}
  

void draw_cell(int y_pos, int x_pos, playfield *field) {
  insert_char(y_pos, x_pos, CELL, field);
}

void draw_point_update_win(WINDOW *mainwin, WINDOW *fieldwin, int y_pos, int x_pos, char c, playfield *field) {
  insert_char(y_pos, x_pos, c, field);
  mvwaddch(fieldwin, y_pos, x_pos, c);
  wmove(fieldwin, y_pos, x_pos);
  update_display(mainwin, fieldwin);
}

void delete_char_update_win(WINDOW *mainwin, WINDOW *fieldwin, int y_pos, int x_pos, playfield *field) {
  delete_char(y_pos, x_pos, field);
  wdelch(fieldwin);
  wclrtoeol(fieldwin);
  for (int i = 0; i < field->cols; i++) {
    char c = field->board[field_index(y_pos, i, *field)];
    if (c != 0) {
      mvwaddch(fieldwin, y_pos, i, c);
    }
  }
  wmove(fieldwin, y_pos, x_pos);
  update_display(mainwin, fieldwin);
}

void clear_messages(WINDOW *mainwin) {
  wmove(mainwin, 1, (getmaxx(mainwin)-1)/2 + 10);
  wclrtoeol(mainwin);
  draw_title(mainwin);
}

int get_int_input(WINDOW *mainwin, char *prompt) {
  clear_messages(mainwin);

  wtimeout(mainwin, -1); // make sure it waits for input

  curs_set(1);
  int from_right = 8;
  wmove(mainwin, 1, (getmaxx(mainwin) - 1) - strlen(prompt) - from_right);
  waddstr(mainwin, prompt);
  echo();
  int number;
  char inputstr[from_right - 1];
  mvwgetnstr(mainwin, 1, (getmaxx(mainwin) - 1) - from_right, inputstr, from_right - 1);
  number = atoi(inputstr);
  noecho();

  // overwrite terminal in area
  clear_messages(mainwin);
  return number;
}

char * get_string_input(WINDOW *mainwin, char *prompt) {
  clear_messages(mainwin);

  wtimeout(mainwin, -1); // make sure it waits for input

  curs_set(1);
  int from_right = 18;
  wmove(mainwin, 1, (getmaxx(mainwin) - 1) - strlen(prompt) - from_right);
  waddstr(mainwin, prompt);
  echo();
  char * inputstr = malloc((from_right)*sizeof(char));
  mvwgetnstr(mainwin, 1, (getmaxx(mainwin) - 1) - from_right, inputstr, from_right - 1);
  noecho();

  // overwrite terminal in area
  clear_messages(mainwin);
  return inputstr;

}

void print_message(WINDOW *mainwin, char *prompt) {
  clear_messages(mainwin);

  // this should check if the message will fit. I could optimize this but I want to make it explicit in what it does.
  const char * welcome_message = "Welcome to conways game of life!\0";
  if (strlen(welcome_message) + 2 > getmaxx(mainwin) - 1 - strlen(prompt) - 3) {
    char *msg = "Message too long, increase screen size\0";
    mvwaddstr(mainwin, 1, (getmaxx(mainwin) - 1) - strlen(msg) - 3, msg);
    return;
  }


  mvwaddstr(mainwin, 1, (getmaxx(mainwin) - 1) - strlen(prompt) - 3, prompt);
  wrefresh(mainwin);
}
