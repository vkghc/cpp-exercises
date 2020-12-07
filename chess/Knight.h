#include "Piece.h"

#ifndef KNIGHT_H
#define KNIGHT_H


class Knight : public Piece {
  
 protected:
  
  bool checkPattern(int s[LOC_SIZE], int f[LOC_SIZE]) override;

  bool checkNoObstacleOnWay(int s[LOC_SIZE], int f[LOC_SIZE]) override;

 public:

  Knight(std::string const unicode_macro);

  ~Knight(){}; 
}; 

#endif
