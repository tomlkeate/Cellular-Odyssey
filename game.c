#include "game.h"
#include "draw.h"
#include "filehandler.h"
#include "config.h"

#define TRUE 1
#define FALSE 0

// create a global variable so you can update the game speed as you play
short UPDATESPEED = GAME_SPEED;

void game_loop(WINDOW *mainwin, WINDOW *fieldwin, playfield *field) {
  int playing = TRUE, animation = FALSE;

  // draw the title on the main window
  draw_title(mainwin);
  print_message(mainwin, "Edit Mode");

  // update the cursor onto the fieldwin
  wmove(fieldwin, 0, 0);
  touchwin(mainwin);
  wrefresh(fieldwin);
  wrefresh(mainwin);

  // declare cursor x_pos and y_pos
  int x_pos, y_pos;

  char *filename_str,  *title_str;

  GAMELOOP:while (playing) {

    // if in animation goto animation
    if (animation) 
      goto ANIMATIONLOOP;

    // show cursor
    curs_set(1);

    // set wait for input for curses to none 
    wtimeout(fieldwin, -1);

    // get and react to input
    int c = wgetch(fieldwin);
    switch (c) {
      case 27:
      case 'q':
        draw_quit(mainwin);
        playing = FALSE;
        #if  EXIT_BELL == TRUE
        beep();
        #endif
        wait(1000);
        break;
      case 'n':
        clear_messages(mainwin);
        int n = get_int_input(mainwin, "How many random spots? ");
        generate_random_spots(n, field);
        draw_field(mainwin, fieldwin, *field);
        break;
      case 'f':
        fill_field(field); 
        draw_field(mainwin, fieldwin, *field);
        break;
      case KEY_UP:
      case 'k':
        getyx(fieldwin, y_pos, x_pos);
        if (y_pos - 1 >= 0) wmove(fieldwin, y_pos-1, x_pos);
        break;
      case KEY_DOWN:
      case 'j':
        getyx(fieldwin, y_pos, x_pos);
        if (y_pos + 1 < getmaxy(fieldwin)) wmove(fieldwin, y_pos+1, x_pos);
        break;
      case KEY_RIGHT:
      case 'l':
        getyx(fieldwin, y_pos, x_pos);
        if (x_pos + 1 < getmaxx(fieldwin)) wmove(fieldwin, y_pos, x_pos+1);
        break;
      case KEY_LEFT:
      case 'h':
        getyx(fieldwin, y_pos, x_pos);
        if (x_pos - 1 >= 0) wmove(fieldwin, y_pos, x_pos-1);
        break; 
      case 'x':
        getyx(fieldwin, y_pos, x_pos);
        delete_char_update_win(mainwin, fieldwin, y_pos, x_pos, field);
        break;
      case 'i':
        getyx(fieldwin, y_pos, x_pos);
        draw_point_update_win(mainwin, fieldwin, y_pos, x_pos, CELL, field);
        break;
      case 'p':
        clear_messages(mainwin);
        animation = TRUE;
        print_message(mainwin, "Playing Mode");
        goto ANIMATIONLOOP;
        break;
      case 'c':
        clear_field(field);
        wclear(fieldwin);
        break;
      case '?':
        // print help message using a new pad
        break;
      case 'W':
      case 'w':
        getyx(fieldwin, y_pos, x_pos);
        filename_str = get_string_input(mainwin, "Write filename: ");
        title_str = get_string_input(mainwin, "Title of design: ");
        curs_set(0);
        write_to_file(filename_str, title_str, *field);
        print_message(mainwin, "Writing to file...");
        free(filename_str);
        free(title_str);
        wait(250);
        print_message(mainwin, "Edit Mode");
        wmove(fieldwin, y_pos, x_pos);
        curs_set(1);
        break;
      case 'R':
      case 'r':
        getyx(fieldwin, y_pos, x_pos);
        filename_str = get_string_input(mainwin, "Read filename: ");
        curs_set(0);
        if (read_from_file(mainwin, fieldwin, filename_str, field) == 0) {
          print_message(mainwin, "Reading from file...");
        } else {
          wait(500);
          print_message(mainwin, "Couldn't read from file");
          wait(500);
        }
        free(filename_str);
        wait(250);
        print_message(mainwin, "Edit Mode");
        wmove(fieldwin, y_pos, x_pos);
        curs_set(1);
        break;
      case KEY_RESIZE:
        resize_display(mainwin, fieldwin, &playing, field);
        break;
      default:
        getyx(fieldwin, y_pos, x_pos);
        curs_set(0);
        print_message(mainwin, "Type ? for help!");
        wait(1000);
        clear_messages(mainwin);
        curs_set(1);
        wmove(fieldwin, y_pos, x_pos);
        break;
    }
    touchwin(mainwin);
    wrefresh(mainwin);
  }

  ANIMATIONLOOP:while (animation) {

    // hide cursor
    curs_set(0);
    wtimeout(fieldwin, UPDATESPEED); // speed of field update default 100 configurable in config.h
    int key = wgetch(fieldwin);
    switch(key) {
      case ERR:
        break;
      case 'f':
      case KEY_UP:
      case 'k':
      case KEY_DOWN:
      case 'j':
      case KEY_RIGHT:
      case 'l':
      case KEY_LEFT:
      case 'h':
      case 'x':
      case 'i':
      case 'R':
      case 'r':
        print_message(mainwin, "Cannot use in Playing Mode");
        wait(500);
        print_message(mainwin, "Playing Mode");
        break;
      case 'p':
        getyx(fieldwin, y_pos, x_pos);
        animation = FALSE;
        print_message(mainwin, "Edit Mode");
        wmove(fieldwin, y_pos, x_pos);
        touchwin(mainwin);
        wrefresh(mainwin);
        goto GAMELOOP;
        break;
      case 27:
      case 'q':
        draw_quit(mainwin);
        playing = FALSE;
        animation = FALSE;
        beep();
        wait(1000);
        break;
      case '=':
      case '+':
        if ((UPDATESPEED - 1) > 1) {
          print_message(mainwin, "Cell Speed++");
          wait(100);
          clear_messages(mainwin);
          UPDATESPEED--;
        }
        break;
      case '_':
      case '-':
        if ((UPDATESPEED + 1) < 500) {
          print_message(mainwin, "Cell Speed--");
          UPDATESPEED++;
          wait(100);
          clear_messages(mainwin);
        }
        break;
      case '?':
        // print help menu
        break;
      case KEY_RESIZE:
        resize_display(mainwin, fieldwin, &playing, field);
        break;
      default:
        print_message(mainwin, "Type ? for help.");
        wait(500);
        print_message(mainwin, "Playing Mode");
        break;
    }
    update_cells(field);
    draw_field(mainwin, fieldwin, *field);
  }
}

void generate_random_spots(int n, playfield *field) {
  int x,y;
  for (int i = 0; i < n; i++) {
    x = rand() % (field->cols); 
    y = rand() % (field->rows); 
    draw_cell(y, x, field);
  }
}

void update_cells(playfield *field) {
  // This function updates the cells by making a copy of the board and updating it and then copying it over

  playfield newField = create_new_field_from_field(*field);
  // check top left cell
  if (field->board[field_index(0, 0, *field)] == CELL) {
    if (check_left_top_corner_cell(0, 0, *field) < 2)
      delete_char(0, 0, &newField);
  } else {
    if (check_left_top_corner_cell(0, 0, *field) == 3)
      draw_cell(0, 0, &newField);
  }

  // check top row
  for (int col = 1, row = 0; col < field->cols - 1; col++) {
    int neighbours = check_top_cell(row, col, *field);
    if (field->board[field_index(row, col, *field)] == CELL) {
      if (neighbours < 2 || neighbours > 3)
          delete_char(row, col, &newField);
    } else if (neighbours == 3) {
        draw_cell( row, col, &newField);
    }
  }

  // check top right corner
  if (field->board[field_index(0, field->cols-1, *field)] == CELL) {
    if (check_right_top_corner_cell(0, field->cols-1, *field) < 2)
      delete_char(0, field->cols-1, &newField);
  } else if (check_right_top_corner_cell(0, field->cols-1, *field) == 3) {
      draw_cell(0, field->cols-1, &newField);
  }

  // check right side
  for (int col = field->cols-1, row = 1; row < field->rows - 1; row++) {
    int neighbours = check_right_cell(row, col, *field);
    if (field->board[field_index(row, col, *field)] == CELL) {
        if (neighbours < 2 || neighbours > 3)
          delete_char(row, col, &newField);
    } else if (neighbours == 3) {
        draw_cell(row, col, &newField);
    }
  }

  // check bottom right corner cell
  if (field->board[field_index(field->rows-1, field->cols-1, *field)] == CELL) {
    if (check_right_bottom_corner_cell(field->rows-1, field->cols-1, *field) < 2)
      delete_char(field->rows-1, field->cols-1, &newField);
  } else if (check_right_bottom_corner_cell(field->rows-1, field->cols-1, *field) == 3) {
      draw_cell(field->rows-1, field->cols-1, &newField);
  }

  // check bottom row
  for (int col = field->cols-2, row = field->rows-1; col > 0; col--) {
    int neighbours = check_bottom_cell(row, col, *field);
    if (field->board[field_index(row, col, *field)] == CELL) {
        if (neighbours < 2 || neighbours > 3)
          delete_char(row, col, &newField);
    } else if (neighbours == 3) {
        draw_cell(row, col, &newField);
    }
  }

  // check bottom left corner cell
  if (field->board[field_index(field->rows-1, 0, *field)] == CELL) {
    if (check_left_bottom_corner_cell(field->rows-1, 0, *field) < 2)
      delete_char(field->rows-1, 0, &newField);
  } else if (check_left_bottom_corner_cell(field->rows-1, 0, *field) == 3) {
      draw_cell(field->rows-1, 0, &newField);
  }

  // check left side
  for (int col = 0, row = field->rows-2; row > 0; row--) {
    int neighbours = check_left_cell(row, col, *field);
    if (field->board[field_index(row, col, *field)] == CELL) {
        if (neighbours < 2 || neighbours > 3)
          delete_char(row, col, &newField);
    } else if (neighbours == 3) {
        draw_cell(row, col, &newField);
    }
  }

  // check normal cells
  for (int row = 1; row < field->rows - 1; row++) {
    for (int col = 1; col < field->cols - 1; col++) {
      int neighbours = check_normal_cell(row, col, *field);
      if (field->board[field_index(row, col, *field)] == CELL) {
        if (neighbours < 2 || neighbours > 3) {
         // Any live cell with fewer than two live neighbours dies, as if by underpopulation
          delete_char(row, col, &newField);
        }
      } else if (neighbours == 3) {
        draw_cell( row, col, &newField);
      }
    }
  }

  // copy elements back
  copy_field(field, newField);

  // free clone board
  free_field(&newField);
}

int check_normal_cell(int row, int col, const playfield field) {
  int neighbours = 0;
  // Any live cell with fewer than two live neighbours dies, as if by underpopulation.
  if ( field.board[field_index(row+1, col, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row+1, col-1, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row+1, col+1, field)] == CELL ) neighbours++;

  if ( field.board[field_index(row-1, col, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row-1, col-1, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row-1, col+1, field)] == CELL ) neighbours++;

  if ( field.board[field_index(row, col+1, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row, col-1, field)] == CELL ) neighbours++;

  return neighbours;
}

int check_left_top_corner_cell(int row, int col, const playfield field) {
  // row: 0 col: 0
  int neighbours = 0;
  if ( field.board[field_index(row+1, col, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row+1, col+1, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row, col+1, field)] == CELL ) neighbours++;
  return neighbours;
}

int check_top_cell(int row, int col, const playfield field) {
  int neighbours = 0;
  // Any live cell with fewer than two live neighbours dies, as if by underpopulation.
  if ( field.board[field_index(row+1, col, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row+1, col-1, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row+1, col+1, field)] == CELL ) neighbours++;

  if ( field.board[field_index(row, col+1, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row, col-1, field)] == CELL ) neighbours++;

  return neighbours;
}


int check_right_top_corner_cell(int row, int col, const playfield field) {
  // row: 0 col: field.cols - 1
  int neighbours = 0;

  // Any live cell with fewer than two live neighbours dies, as if by underpopulation.
  if ( field.board[field_index(row+1, col, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row+1, col-1, field)] == CELL ) neighbours++;

  if ( field.board[field_index(row, col-1, field)] == CELL ) neighbours++;

  return neighbours;
}

int check_right_cell(int row, int col, const playfield field) {
  int neighbours = 0;
  // Any live cell with fewer than two live neighbours dies, as if by underpopulation.

  if ( field.board[field_index(row+1, col, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row+1, col-1, field)] == CELL ) neighbours++;

  if ( field.board[field_index(row-1, col, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row-1, col-1, field)] == CELL ) neighbours++;

  if ( field.board[field_index(row, col-1, field)] == CELL ) neighbours++;

  return neighbours;
}

int check_right_bottom_corner_cell(int row, int col, const playfield field) {
  int neighbours = 0;
  // Any live cell with fewer than two live neighbours dies, as if by underpopulation.
  if ( field.board[field_index(row-1, col, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row-1, col-1, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row, col-1, field)] == CELL ) neighbours++;
  return neighbours;
}

int check_bottom_cell(int row, int col, const playfield field) {
  int neighbours = 0;
  // Any live cell with fewer than two live neighbours dies, as if by underpopulation.
  if ( field.board[field_index(row-1, col, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row-1, col-1, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row-1, col+1, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row, col+1, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row, col-1, field)] == CELL ) neighbours++;

  return neighbours;
}

int check_left_bottom_corner_cell(int row, int col, const playfield field) {
  int neighbours = 0;

  // Any live cell with fewer than two live neighbours dies, as if by underpopulation.
  if ( field.board[field_index(row-1, col, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row-1, col+1, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row, col+1, field)] == CELL ) neighbours++;

  return neighbours;
}

int check_left_cell(int row, int col, const playfield field) {
  int neighbours = 0;
  // Any live cell with fewer than two live neighbours dies, as if by underpopulation.
  if ( field.board[field_index(row+1, col, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row+1, col+1, field)] == CELL ) neighbours++;

  if ( field.board[field_index(row-1, col, field)] == CELL ) neighbours++;
  if ( field.board[field_index(row-1, col+1, field)] == CELL ) neighbours++;

  if ( field.board[field_index(row, col+1, field)] == CELL ) neighbours++;

  return neighbours;
}
