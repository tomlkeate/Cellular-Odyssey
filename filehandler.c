#include <stdio.h>
#include <string.h>
#include "filehandler.h"
#include "draw.h"

// this is able to open and export files
int write_to_file(char *filename, char *title, const playfield field) {
  FILE *file_ptr;

  char *file_type = ".conways";
  char real_file_name[strlen(filename) + strlen(file_type)];
  strcpy(real_file_name, filename);
  strcat(real_file_name, file_type);

  file_ptr = fopen(real_file_name, "w+");

  fprintf(file_ptr, "This file is for the conways game of life simulator designed by https://github.com/tomlkeate\n");
  fprintf(file_ptr, "Title: %s\n", title);

  fprintf(file_ptr, "ROWS:%i\nCOLS:%i\n", field.rows, field.cols);

  for (int i = 0; i < field.rows; i++) {
    for (int j = 0; j < field.cols; j++) {
      if (field.board[field_index(i, j, field)] == CELL)
        fputc('*', file_ptr);
      else
        fputc('.', file_ptr);
    }
    fprintf(file_ptr, "\n");
  }

  fclose(file_ptr);
  return 0;
}

int read_from_file(WINDOW *mainwin, WINDOW *fieldwin, char *filename, playfield *field) {
  FILE *read_file_ptr;

  char *file_type = ".conways";
  char real_file_name[strlen(filename) + strlen(file_type)];
  strcpy(real_file_name, filename);
  strcat(real_file_name, file_type);

  read_file_ptr = fopen(real_file_name, "r");

  if (read_file_ptr == NULL) {
    print_message(mainwin, "File doesn't exist");
    return -1;
  }

  char cNewLine;
  if (!fscanf(read_file_ptr, "This file is for the conways game of life simulator designed by https://github.com/tomlkeate%c", &cNewLine)) {
    print_message(mainwin, "Missing filetype info. Skipping...");
  }

  char *title = calloc(22, sizeof(char));
  title[0 * sizeof(char)] = '\0';
  title[21 * sizeof(char)] = '\0';
  if (!fscanf(read_file_ptr, "Title: %20[A-Za-z ]\n", title)) {
    print_message(mainwin, "Missing Title. Skipping...");
  }
  draw_game_title(mainwin, title);

  int Rows=-1, Cols=-1;
  if (!fscanf(read_file_ptr, "ROWS:%d\n", &Rows)) {
    //print_message(mainwin, "Rows indentifer is in wrong format or doesn't exist.");
  }

  if (!fscanf(read_file_ptr, "COLS:%d\n", &Cols)) {
    //print_message(mainwin, "Cols indentifer is in wrong format or doesn't exist.");
  }

  playfield read_field = create_new_field_from_field(*field);

  // find out the cols and rows if not found out before
  if (Rows == -1 || Cols == -1) {
    char c = fgetc(read_file_ptr);
    Cols = 1, Rows = 0;
    while (c != EOF) {
      c = fgetc(read_file_ptr);
      if (c == '\n') {
        Rows++;
      } else if (Rows == 0 && c != '\t') {
        Cols++;
      }
    }
  }
  // reset the file ptr to the start of the saved field
  fseek(read_file_ptr, -(Cols*Rows + Rows), SEEK_CUR);

  int smallest_row = (field->rows < Rows)? field->rows : Rows;
  int smallest_cols = (field->cols < Cols)? field->cols : Cols;

  for (int row = 0; row < smallest_row; row++) {
    for (int col = 0; col <= smallest_cols; col++) {
      char c;
      if (col == smallest_cols) {
        c = fgetc(read_file_ptr);
        while (c != '\n')
          c = fgetc(read_file_ptr);
        continue;
      } else {
        c = fgetc(read_file_ptr);
      }

      if (c == '*') {
        read_field.board[field_index(row, col, *field)] = CELL;
      } else if (c == '.') {
        read_field.board[field_index(row, col, *field)] = 0;
      } else if (c == '\n') {
        col--;
        continue;
      } else if (c == '\t') {
        read_field.board[field_index(row, col, *field)] = 0;
      } else {
        char output_string[57 + 20];
        sprintf(output_string, "File Reading Error. Row: %d Col: %d Didn't understand \"%c\"", row, col, c);
        print_message(mainwin, output_string);
        free_field(&read_field);
        fclose(read_file_ptr);
        return -1;
      }
    }
  }

  // update field and free the copy
  copy_field(field, read_field);
  free_field(&read_field);

  fclose(read_file_ptr);

  draw_field(mainwin, fieldwin, *field);
  return 0;
}
