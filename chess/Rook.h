#include "Piece.h"

#ifndef ROOK_H
#define ROOK_H


class Rook : public virtual Piece {
  
 protected:
  
  bool checkPattern(int s[LOC_SIZE], int f[LOC_SIZE]) override;

  bool checkNoObstacleOnWay(int s[LOC_SIZE], int f[LOC_SIZE]) override;
  
 public:

  Rook(std::string const unicode_macro); 

  ~Rook(){}; 
}; 

#endif
