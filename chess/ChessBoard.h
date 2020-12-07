/* Header for class chess board */
#include "Piece.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"
#include <iostream>


#ifndef CHESSBOARD_H
#define CHESSBOARD_H

std::ostream& operator<<(std::ostream& o, const Piece* p);

class ChessBoard {
 private:
  
  // data members
  Piece* current_board_[8][8];
  Color in_turn_;
  int move_num_;
  int en_passant_; 
  Color in_check_;
  Color in_mate_;
  Color in_stalemate_; 
  Piece* white_king_;
  Piece* black_king_;

  friend std::ostream& operator<<(std::ostream& o, const Piece& p);

  // private (helper) methods

  void initialiseNewBoard();

  void destroyOldBoard(); 

  void passBoardCopy(int s[LOC_SIZE]);
  
  bool checkCoordinates(std::string start, std::string fin);
  
  void convertCoord(std::string const location, int loc[LOC_SIZE]);

  void makeMove(int s[LOC_SIZE], int f[LOC_SIZE]);

  bool checkOwnKingSafety(int s[LOC_SIZE], int f[LOC_SIZE]);

  void setInCheckFlag(Piece* own_king);

  int checkCastleRequest(Piece* own_king,
			 int s[LOC_SIZE], int f[LOC_SIZE]); 

  bool checkAnyPossibleMoves(Color color, Piece* king);

  // helper to print the output
  void printOutput(int event,
		   std::string const start, std::string const fin,
		   Piece* p);

  
 public:
  
  ChessBoard();

  void submitMove(std::string const start, std::string const fin);

  void resetBoard(); // TO DO!!!

  void destroyPieces(); 
  
  ~ChessBoard();
};

#endif
