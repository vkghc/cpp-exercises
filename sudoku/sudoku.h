/* Header file for sudoku functions */

#ifndef SUDOKU_H
#define SUDOKU_H

/* ~~~ PRE-SUPPLIED FUNCTIONS ~~~ */

/* loads a Sudoku board from a file */
void load_board(const char* filename, char board[9][9]);

/* internal helper function */
void print_frame(int row);

/* function to display a Sudoku board */
void display_board(const char board[9][9]);



/* ~~~ VESKO'S FUNCTIONS START HERE ~~~  */

/* ~~~Question 1~~~ */

/* Function returns true if every cell on the board
   is , possibly invalid, digit. */
bool is_complete(char board[9][9]);



/* ~~~Question 2~~~ */

/* Helper to convert coordinate of type 'B2' to corrdinates which 
can be stored an array of type: int a[2] = {1, 1} and applied to the 
board like so: board[ a[0] ] [ a[1] ] */
void convert_coordinates(const char letter_coord[2], int int_coord[2]);

/* Helper to check if 'digit' is a legal, 1-9, digit */
bool legal_digit(char digit);

/* Helper to check if digit already appears in the row and column 
   which the coordinates ic[2] belong to */ 
bool check_duplicate_line(const int  ic[2], char digit, const char board[9][9]);

/* Helper to define the starting integer coordinate of the 3x3 mini-square 
   containing the integer coordinates ic[2]. So, {1,1} belongs
   to 3x3 mini-square beginning board[0][0] */
void define_minisquare(const int ic[2], int start_minisquare[2]);

/* Helper to check if a given digit already appears in the 3x3 mini-square
   with starting integer  coordinates start_minisquare[2] */
bool check_duplicate_minisquare(char digit, const int start_minisquare[2], const char board[9][9]);

/* Function to decide whether a given move is valid. If yes, return 'ture' and 
   change board accordingly. Otherwise, return 'false' and do not change board. */ 
bool make_move(const char letter_coord[], char digit, char board[9][9]);

/* Test function, to test whether make_move() works
   with different input parameters -- for Q2 tests only! */
void test_make_move(const char letter_coordinates[], char digit, char board[9][9]);



/* ~~~Question 3~~~ */

/* Function to save board[][] to a file with name specified by filename */ 
bool save_board(const char *filename, const  char board[9][9]);



/* ~~~Question 4~~~ */

/* Helper: same as make_move, but does not change board */ 
bool check_move(const int ic[2], char digit, const char board[9][9]); // ic stands for integer coordinates (ie, actual board[][] indices)

/* Helper: for a given cell, fill string possible_moves with all legal moves given board */ 
void possible_moves_cell(const int ic[2], const char board[9][9], const char test_digits[], char possible_moves[]);

/* Helper function to find the first empty cell and modify coordinates in ic[] */
bool find_empty_cell(int ic[2], const char board[9][9]);

/* Function to solve the board recursively and return true or return false if no solution exists */
bool solve_board(char board[9][9]);



/* ~~~Question 5~~~ */

/* Helper: same as load_board but boolean -- returns true if 
   board loaded successfully */
bool load_board_bool(const char* filename, char board[9][9]);

/* Helper: Same as solve_board() but also counts how many recursive function calls are needed
   to find/disprove solution */
bool solve_board_count(char board[9][9], int& recursion_counter); 

/* Function to load board from a file (filename) and attempt to solve it.
   If solution exists, board is printed. The time taken to complete the function is
   printed in milliseconds. Number of recursive calls made is also printed. 
   The solved baord is saved in a solution-*.dat file. */
void sudoku_solver(const char* filename, char board[9][9]);

#endif
