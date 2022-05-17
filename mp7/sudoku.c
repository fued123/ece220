#include "sudoku.h"

//-------------------------------------------------------------------------------------------------
// Start here to work on your MP7
//-------------------------------------------------------------------------------------------------

/*
The purpose of this program is to implement a solver for a sudoku puzzle
using recursive methods. 2 of these functions actually call on some of the
other main functions. The first three functions are used to determine if a
value is duplicated in the rows, columns or each 3x3 grid of the 9x9 sudoku table.
The fourth function is used to test for all 3 cases simulataneously to simplify the
implementation. The fifth function is a private function that I created, to keep the
solve_sudoku function concise and neat while using some of the pseudocode provided.
The final function that I wrote is the solve_sudoku function, which uses the fifth,
private function to check if every cell has a number.
*/

// You are free to declare any private functions if needed.

// Function: is_val_in_row
// Return true if "val" already existed in ith row of array sudoku.
int is_val_in_row(const int val, const int i, const int sudoku[9][9]) {

  assert(i>=0 && i<9);

  // BEG TODO
    for (int j = 0; j < 9; j++){
      if (sudoku[i][j] == val){ //iterates through row to check for duplicate value
        return 1;
      }
    }
  return 0;
  // END TODO
}

// Function: is_val_in_col
// Return true if "val" already existed in jth column of array sudoku.
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {

  assert(j>=0 && j<9);

  // BEG TODO
    for (int i = 0; i < 9; i++){
      if (sudoku[i][j] == val){//iterates to column to check for duplicate value
          return 1; 
      }
    }
  return 0;
  // END TODO
}

// Function: is_val_in_3x3_zone
// Return true if val already existed in the 3x3 zone corresponding to (i, j)
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {
   
  assert(i>=0 && i<9);
  
  // BEG TODO
    int x = i/3; //truncating to find 3x3 column
    int y = j/3; //truncating to find 3x3 row
    for (int a = x*3; a < x*3+3; a++){ //testing for 3 rows
      for (int b = y*3; b < y*3+3; b++){ //testing for 3 columns
        if (sudoku[a][b] == val){ //testing if value is a duplicate in the 3x3 grid
          return 1;
        }
      }
    }
  return 0;
  // END TODO
}

// Function: is_val_valid
// Return true if the val is can be filled in the given entry.
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i>=0 && i<9 && j>=0 && j<9);

  // BEG TODO
  //This if statement below is a simultaneous test of the three cases for invalid input
    if (is_val_in_row(val, i, sudoku) || is_val_in_col(val, j, sudoku) || is_val_in_3x3_zone(val, i, j, sudoku)){
      return 0;
    }
  return 1;
  // END TODO
}
/* 
This function checks if every cell in the sudoku
table/board has been assigned a number/value. It returns 1
if a cell has no assignment. This helps to keep the function
solve_sudoku concise and neat.
*/
int notassigned(int sudoku[9][9], int &row, int &col){
  for (row = 0; row < 9; row++){
    for (col = 0; col < 9; col++){
      if (sudoku[row][col] == 0){
        return 1;
      }
    }
  }
  return 0;
}

// Procedure: solve_sudoku
// Solve the given sudoku instance.
int solve_sudoku(int sudoku[9][9]) {

  // BEG TODO.
  int row, col, num;
  if (!notassigned(sudoku, row, col)) {//checking if each cell has an assigned value
    return 1;
  }
  for (num = 1; num <= 9; num++){
    if (is_val_valid(num, row, col, sudoku) && sudoku[row][col] == 0){
      sudoku[row][col] = num;
      if (solve_sudoku(sudoku)){
        return 1;
      }
      sudoku[row][col] = 0;
    }
  }
  return 0;
  // END TODO.
}

// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9])
{
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      printf("%2d", sudoku[i][j]);
    }
    printf("\n");
  }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
  FILE *reader = fopen(fpath, "r");
  assert(reader != NULL);
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      fscanf(reader, "%d", &sudoku[i][j]);
    }
  }
  fclose(reader);
}





