#include "Piece.h"

#ifndef BISHOP_H
#define BISHOP_H


class Bishop : public virtual Piece {
  
 protected:
  
  bool checkPattern(int s[LOC_SIZE], int f[LOC_SIZE]) override;

  bool checkNoObstacleOnWay(int s[LOC_SIZE], int f[LOC_SIZE]) override;
  
 public:

  Bishop(std::string const unicode_macro);

  ~Bishop(){}; 
}; 

#endif
