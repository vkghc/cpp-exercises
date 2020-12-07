#include "Piece.h"

#ifndef PAWN_H
#define PAWN_H

class Pawn : public Piece {
  
 protected:

  int takes_in_passing_; 

  bool checkPattern(int s[LOC_SIZE], int f[LOC_SIZE]) override;

  bool checkNoObstacleOnWay(int s[LOC_SIZE], int f[LOC_SIZE]) override;

 public:

  Pawn(std::string const unicode_macro);

  int getSpecialFeature() override; 
  
  ~Pawn(){};
}; 

#endif
