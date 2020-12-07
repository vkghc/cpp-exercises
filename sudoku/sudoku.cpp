#include "sudoku.h"
#include <iostream>
#include <fstream> 
#include <cstdio> 
#include <cstring> 
#include <cassert>
#include<chrono> // commands using this lib taken from cppreference.com (see l 426)

using namespace std;

/* ~~~ PRE-SUPPLIED FUNCTIONS: PROVIDED BY MY INSTRUCTORS (NOT MY CODE) ~~~ */

/* loads a Sudoku board from a file */
void load_board(const char* filename, char board[9][9]) {

  cout << "Loading Sudoku board from file '" << filename << "'... ";

  ifstream in(filename); // initialising an fstream variable
  if (!in) // check if flags are false
    cout << "Failed!" << endl;
  assert(in); 

  char buffer[512]; //more chars than needed? 

  int row = 0;
  in.getline(buffer,512);
  while (in && row < 9) {
    for (int n=0; n<9; n++) { // populates first row
      assert(buffer[n] == '.' || isdigit(buffer[n]));
      board[row][n] = buffer[n];
    }
    row++;
    in.getline(buffer,512); // opens subsequent rows 
  }

  cout << ((row == 9) ? "Success!" : "Failed!") << endl;
  in.close(); // don't forget to close instream! 
  assert(row == 9);
}

/* internal helper function */
void print_frame(int row) {
  if (!(row % 3))
    cout << "  +===========+===========+===========+" << endl;
  else
    cout << "  +---+---+---+---+---+---+---+---+---+" << endl;
}

/* internal helper function */
void print_row(const char* data, int row) { // 'data' is array for each row, not a table
  cout << (char) ('A' + row) << " ";
  for (int i=0; i<9; i++) {
    cout << ( (i % 3) ? ':' : '|' ) << " ";
    cout << ( (data[i]=='.') ? ' ' : data[i]) << " ";
  }
  cout << "|" << endl;
}

/* function to display a Sudoku board */
void display_board(const char board[9][9]) {
  cout << "    ";
  for (int r=0; r<9; r++) 
    cout << (char) ('1'+r) << "   ";
  cout << endl;
  for (int r=0; r<9; r++) {
    print_frame(r);
    print_row(board[r],r);
  }
  print_frame(9);
}



/* ~~~ MY CODE STARTS HERE ~~~  */

/* ~~~Question 1~~~ */

/* Function returns true if every cell on the board
   is , possibly invalid, digit. */
bool is_complete(char board[9][9])
{
  for(int row = 0; row < 9; row++)
    for(int col = 0; col < 9; col++)
      if(board[row][col] == '.')
	return false;
  return true;
}



/* ~~~Question 2~~~ */

/* Helper to convert coordinate of type 'B2' to corrdinates which 
can be stored an array of type: int a[2] = {1, 1} and applied to the 
board like so: board[ a[0] ] [ a[1] ] */
void convert_coordinates(const char letter_coord[2], int int_coord[2])
{
  // ACSII codes for uppercase letters A-I: 65-73
  // ASCII for 1-9 digits: 49-57

  for(int i = 0; (i + 65) <= 73; i++)
    if(letter_coord[0] == (char)(i + 65) ) 
      int_coord[0] = i;

  for(int i = 0; (i + 49) <= 57; i++)
    if(letter_coord[1] == (char)(i + 49) ) 
      int_coord[1] = i;      
}

/* Helper to check if 'digit' is a legal, 1-9, digit */
bool legal_digit(char digit)
{
  // check if legal digit
  for(int i = 0; (i + 49) <= 57; i++)
      if(digit == (char)(i + 49) )
	return true;
  return false;
}

/* Helper to check if digit already appears in the row and column 
   which the coordinates ic[2] belong to */ 
bool check_duplicate_line(const int  ic[2], char digit, const char board[9][9])
{
  for(int i = 0; i < 9; i++)
    {
      if(digit == board[ ic[0] ][i])
	return true;
      
      if(digit == board[i][ ic[1] ])
	return true;
    }
  
  return false;
}

/* Helper to define the starting integer coordinate of the 3x3 mini-square 
   containing the integer coordinates ic[2]. So, {1,1} belongs
   to 3x3 mini-square beginning board[0][0] */
void define_minisquare(const int ic[2], int start_minisquare[2])
{
  for(int i = 0; i < 2; i++)
    {
      if( ic[i] % 3 == 0)
	start_minisquare[i] = ic[i];

      else if( (ic[i] - 1) % 3 == 0)
	start_minisquare[i] = ic[i] - 1;

      else
	start_minisquare[i] = ic[i] - 2;
    }
}

/* Helper to check if a given digit already appears in the 3x3 mini-square
   with starting integer  coordinates start_minisquare[2] */
bool check_duplicate_minisquare(char digit, const int start_minisquare[2], const char board[9][9])
{
  int a = start_minisquare[0];
  int b = start_minisquare[1];
  
  for(int i = a; i < (a + 3); i++)
    
    for(int j = b; j < (b + 3); j++)
      
      if(digit == board[i][j])
	return true;

  return false; 
}

/* Function to decide whether a given move is valid. If yes, return 'ture' and 
   change board accordingly. Otherwise, return 'false' and do not change board. */ 
bool make_move(const char letter_coord[], char digit, char board[9][9])
{
  int ic[2] = {-1, -1}; // array integer coordinates

  // if letter coordinates string has more than 2 characters, this is an invalid move
  int counter = 0;
  while(letter_coord[counter]!='\0')
    counter++;
  if( counter > 3 )
    return false;

  // convert letter coordinates to board[][] indices (ie, integer coordinates)
  convert_coordinates(letter_coord, ic);

  // check if in range
  for(int i = 0; i <2 ; i++)   
    if(ic[i] ==  -1)
      return false;

  // check if digit is legal
  if( !legal_digit(digit) )
    return false; 
  
  // check for duplicates in row or column of given a[2] cell
  if( check_duplicate_line(ic, digit, board) )
    return false;

  // check for duplicate in minisquare which a[] belongs to
  int start_minisquare[2];

  define_minisquare(ic, start_minisquare);

  if( check_duplicate_minisquare(digit, start_minisquare, board) )
    return false;

  // if the move is legal change the board
  board[ ic[0] ][ ic[1] ] = digit;
  
  return true; 
  
}

/* Test function, to test whether make_move() works
   with different input parameters -- for Q2 tests only! */
void test_make_move(const char letter_coordinates[], char digit, char board[9][9])
{
  cout << "Putting '" << digit << "' into ";
  int counter = 0;
  
  while(letter_coordinates[counter]!='\0')
    {
      cout << letter_coordinates[counter];
      counter++;
    }

  cout << " is ";
  if (!make_move(letter_coordinates, digit, board) )
    cout << "NOT a valid move." << endl;
  else
    {
      cout << "a valid move. The board is:" << endl;
      display_board(board);
    }
}



/* ~~~Question 3~~~ */

/* Function to save board[][] to a file with name specified by filename */ 
bool save_board(const char *filename, const  char board[9][9])
{
  // open empty file with name as in filenamex
  ofstream out(filename); // initialise outstream

  if(!out)
    return false;

  // create var that holds a single sudoku line
  char buffer[9];

  // print board to filename line by line 
  for(int i = 0; i < 9; i++)
    {
      for(int j = 0; j < 9; j++)
	buffer[j] = board[i][j];

      out.write(buffer, 9);

      if(!out)
	return false; 

      out.put('\n');
    }

 
  // save and close
  out.close();
  
  return true; 
}



/* ~~~Question 4~~~ */

/* Helper: similar to make_move, but does not change board */ 
bool check_move(const int ic[2], char digit, const char board[9][9]) // ic stands for integer coordinates (ie, actual board[][] indices)
{ 
  // check for duplicates in row or column of given a[2] cell
  if( check_duplicate_line(ic, digit, board) )
    return false;

  // check for duplicate in minisquare which a[] belongs to
  int start_minisquare[2];

  define_minisquare(ic, start_minisquare);

  if( check_duplicate_minisquare(digit, start_minisquare, board) )
    return false;
  
  return true; 
  
}

/* Helper: for a given cell, fill string possible_moves with all legal moves given board */ 
void possible_moves_cell(const int ic[2], const char board[9][9], const char test_digits[], char possible_moves[])
{
  
  int counter = 0, valid_counter = 0;
  
  while(test_digits[counter] != '\0')
    {
      if(check_move(ic, test_digits[counter], board) )
	{
	  possible_moves[valid_counter] = test_digits[counter];
	  valid_counter++;
	}
      counter++;
    }
}


/* Helper function to find the first empty cell and modify coordinates in ic[] */
bool find_empty_cell(int ic[2], const char board[9][9])
{
  for(int i = 0; i < 9; i++)
    for(int j = 0; j < 9; j++)
      {
	if(board[i][j] == '.')
	  {
	    ic[0] = i; ic[1] = j;
	    return true;	
	  }
      }
  
  return false;
}

/* Function to solve the board recursively and return true or return false if no solution exists */
bool solve_board(char board[9][9])
{
  int ic[2];
  int counter = 0;
  char possible_moves[] = {'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
  char test_digits[] = {'1','2','3','4','5','6','7','8','9','\0'}; 
  
  // base case
  // if empty cell cannot be found, we're done!
  if( !find_empty_cell(ic, board) ) // ic[2] stands for integer coordinates
    return true;
  
  // inductive case
  // if empty cell is found, get all legal moves for it.
  possible_moves_cell(ic, board, test_digits, possible_moves);

  // while legal moves exist for cell, try a legal move
  while(possible_moves[counter] != '\0')
    {
      if(check_move(ic, possible_moves[counter], board))
	{
	  board[ ic[0] ][ ic[1] ] = possible_moves[counter];
	  
	  // after you play legal move, get on to the next cell
	  // and apply solve_board() recursively
	  solve_board(board);
	}

      // if complete  solution is found at any point in the
      // recursion, stop, we're done!
      if( is_complete(board) )
	return true;
      
      counter++;
    }
  
  // if there are no legal moves left for given cell,
  // change board cell back to '.' before returning false 	
  board[ ic[0] ][ ic[1] ] = '.'; 
    
  return false; 
}


/* ~~~Question 5~~~ */

/* Helper: same as load_board but boolean -- returns true if 
   board loaded successfully */
bool load_board_bool(const char* filename, char board[9][9])
{
  cout << "Loading Sudoku board from file '" << filename << "'... ";

  ifstream in(filename); // initialising an fstream variable
  if (!in) // check if flags are false
    {
      cout << "Failed!" << endl;
      return false;
    }

  char buffer[512]; // Is that not too many characters??

  int row = 0;
  in.getline(buffer,512);
  while (in && row < 9)
    {
      for (int n=0; n<9; n++)
	{
	  if( !(buffer[n] == '.' || isdigit(buffer[n])) )
	    return false;
	  board[row][n] = buffer[n];
	}
      row++;
      in.getline(buffer,512); // opens subsequent rows 
    }

  cout << ((row == 9) ? "Success!" : "Failed!") << endl;
  in.close(); // don't forget to close! 

  if(!(row == 9))
    return false;

  return true;
}

/* Helper: same as solve_board() but also counts how many recursive function calls are needed
   to find/disprove solution */
bool solve_board_count(char board[9][9], int& recursion_counter)
{
  int ic[2];
  int counter = 0;
  char possible_moves[] = {'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
  char test_digits[] = {'1','2','3','4','5','6','7','8','9','\0'}; 
  
  // base case
  // if empty cell cannot be found, we're done!
  if( !find_empty_cell(ic, board) ) // ic[2] stands for integer coordinates
    return true;
  
  // inductive case
  // if empty cell is found, get all legal moves for it.
  possible_moves_cell(ic, board, test_digits, possible_moves);

  // while legal moves exist for cell, try a legal move
  while(possible_moves[counter] != '\0')
    {
      if(check_move(ic, possible_moves[counter], board))
	{
	  board[ ic[0] ][ ic[1] ] = possible_moves[counter];
	  
	  // after you play legal move, get on to the next cell
	  // and apply solve_board() recursively
	  recursion_counter++; 
	  solve_board_count(board, recursion_counter);
	}

      // if complete  solution is found at any point in the
      // recursion, stop, we're done!
      if( is_complete(board) )
	return true;
      
      counter++;
    }
  
  // if there are no legal moves left for given cell,
  // change board cell back to '.' before returning false 	
  board[ ic[0] ][ ic[1] ] = '.'; 
    
  return false; 
}


/* Function to solve sudoku:
   It first  load board from a file (filename). If the load is successful,
   the function tries to solve the board.
   If solution exists, board is printed to screen and a solution file
   is saved in the local directory. Else, a "no soultion" message 
   is printed to screen. The time taken to complete 
   the function is printed in milliseconds. The number of recursive 
   calls needed is also printed. */
void sudoku_solver(const char* filename, char board[9][9])
{ 
  // code taken from: https://en.cppreference.com/w/cpp/chrono/system_clock/now
  auto start = chrono::system_clock::now();
  //

  // an alternative measure of difficulty is how many times the recursive function
  // solve_board() is called.
  int recursion_count = 0; 
  
  cout<< endl; 
  if(!load_board_bool(filename, board))
    return;
  
  if (solve_board_count(board, recursion_count))
    { 
      cout << "The board has a solution:" << endl;
      display_board(board);

      // save solutoin to an appropriately-named file
      char solution[512] = "solution-"; // leave plenty of space for file name
      strcat(solution, filename); 
      save_board(solution, board);
      cout << "Solution saved to " << solution << "." << endl; 
    }
  
  else
    cout << "Solution could NOT be found."  << endl;
  
  // code taken from: https://en.cppreference.com/w/cpp/chrono/system_clock/now
  auto end = chrono::system_clock::now();
  chrono::duration<double> diff = (end-start)*1000;
  //

  cout.setf(ios::fixed);
  cout.precision(0); 
  cout << "Computation time was: " << diff.count()<< "ms" << endl;
  cout << recursion_count << " recursive calls were made." << endl; 
  cout<< endl;
}


  
  

  
  

    
  
