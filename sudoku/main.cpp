#include <iostream>
#include <cstdio>
#include<chrono> // taken from cppreference.com, see sudoku.cpp:426 for more info
#include "sudoku.h"

using namespace std;

int main() {

  char board[9][9];

  /* This section illustrates the use of the pre-supplied helper functions. */
  cout << "============= Pre-supplied functions =============" << endl << endl;

  cout << "Calling load_board():" << endl;
  load_board("easy.dat", board);

  cout << endl << "Displaying Sudoku board with display_board():" << endl;
  display_board(board);
  cout << "Done!" << endl << endl;

  cout << "=================== Question 1 ===================" << endl << endl;

  load_board("easy.dat", board);
  cout << "Board is ";
  if (!is_complete(board))
    cout << "NOT ";
  cout << "complete." << endl << endl;

  load_board("easy-solution.dat", board);
  cout << "Board is ";
  if (!is_complete(board))
    cout << "NOT ";
  cout << "complete." << endl << endl;

  cout << "=================== Question 2 ===================" << endl << endl;

  load_board("easy.dat", board);

  // Should be OK
  cout << "Putting '1' into I8 is ";
  if (!make_move("I8", '1', board)) 
    cout << "NOT ";
  cout << "a valid move. The board is:" << endl;
  display_board(board);

  // Vesko's tests

  // Should be OK
  test_make_move("H7", '7', board);
  
  // Should fail - invalid range
  test_make_move("Z8", '7', board);   
  
  // Should fail - invalid range 2
  test_make_move("I10", '7', board);
 
  // Should fail - invalid digit
  test_make_move("A1", 'r', board);

  // Should fail - illegal move (row)
  test_make_move("I2", '1', board);

  // Should fail - illegal move (col)
  test_make_move("F8", '1', board);

  // Should fail - illegal move (minisquare)
  test_make_move("I9", '5', board);
  

  cout << "=================== Question 3 ===================" << endl << endl;

  load_board("easy.dat", board);
  if (save_board("easy-copy.dat", board))
    cout << "Save board to 'easy-copy.dat' successful." << endl;
  else
    cout << "Save board failed." << endl;
  cout << endl;

  cout << "=================== Question 4 ===================" << endl << endl;
  
  load_board("easy.dat", board);
  if (solve_board(board)) {
    cout << "The 'easy' board has a solution:" << endl;
    display_board(board);
  } else 
    cout << "A solution cannot be found." << endl;
  cout << endl;

  load_board("medium.dat", board);
  if (solve_board(board)) {
    cout << "The 'medium' board has a solution:" << endl;
    display_board(board);
  } else 
    cout << "A solution cannot be found." << endl;
  cout << endl;

  //  write more tests: I think all needed tests are done in Question 5
	
  cout << "=================== Question 5 ===================" << endl << endl;
  
  // tests 
  sudoku_solver("mystery1.dat", board);
  sudoku_solver("mystery2.dat", board);
  sudoku_solver("mystery3.dat", board);

  // if my suspictoin is correct, easy.dat and medium.dat
  // should solve in less time and with less recursion calls than mystery1.dat
  sudoku_solver("easy.dat", board);
  sudoku_solver("medium.dat", board);
  sudoku_solver("hardest.dat", board);

  return 0;
}
