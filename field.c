#include "field.h"

int field_index(int y, int x, const playfield field) {
  // map the location on the screen to the location in the field board
  if (x >= field.cols || x < 0 || y >= field.rows || y < 0) {
    printf("Out of Bounds! y: %d, x: %d, Row: %d, Cols %d\n", y, x, field.rows, field.cols);
  }
  return (y * field.cols) + x;
}

int copy_field(playfield *field1, const playfield field2) {
  // copy field 2 into field one if field2 is the same size of field1 else return -1
  if (field1->rows != field2.rows || field1->cols != field2.cols) {
    return -1;
  }
  for (int y = 0; y < field2.rows; y++) {
    for (int x = 0; x < field2.cols; x++) {
      field1->board[field_index(y, x, *field1)] = field2.board[field_index(y, x, field2)];
    }
  }
  return 0;
}

playfield create_new_field_from_field(const playfield field) {
  playfield newfield;
  newfield.size = field.size;
  char *ptr = calloc(newfield.size + 2, sizeof(char));
  if (ptr == NULL) {
    printf("ERROR Allocating field: %s:%d", __FILE__, __LINE__);
  }
  ptr[field.size + 1] = '\0';
  newfield.board = ptr;
  newfield.rows = field.rows;
  newfield.cols = field.cols;
  copy_field(&newfield, field);
  return newfield;
}

int free_field(playfield *newfield) {
  if (newfield->board != NULL) {
    free(newfield->board);
    newfield->board = NULL;
    return 0;
  }
  return -1;
}

int wait(int ms) {
  struct timespec remaining, request = { ms / 1000, (ms % 1000) * 1000000};
  nanosleep(&request, &remaining); 
  return 1;
}

void fill_field(playfield *field) {
  for (int i = 0; i < field->rows; i++) {
    for (int j = 0; j < field->cols; j++) {
      field->board[field_index(i, j, *field)] = CELL;
    }
  }
}

void clear_field(playfield *field) {
  for (int row = 0; row < field->rows; row++) {
    for (int col = 0; col < field->cols; col++) {
      field->board[field_index(row, col, *field)] = 0;
    }
  }
}

void insert_char(int y_pos, int x_pos, char c, playfield *field) {
  if (x_pos >= 0 && x_pos < field->cols && y_pos >= 0 && y_pos < field->rows) {
    field->board[field_index(y_pos, x_pos, *field)] = c;
  }
}

void delete_char(int y_pos, int x_pos, playfield *field) {
  if (x_pos >= 0 && x_pos < field->cols && y_pos >= 0 && y_pos < field->rows) {
    insert_char(y_pos, x_pos, 0,  field);
  }
}

