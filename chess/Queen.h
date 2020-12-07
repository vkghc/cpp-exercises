#include "Rook.h"
#include "Bishop.h"

#ifndef QUEEN_H
#define QUEEN_H

class Queen : public Rook, public Bishop {
  
 protected:
  
  bool checkPattern(int s[LOC_SIZE], int f[LOC_SIZE]) override;

  bool checkNoObstacleOnWay(int s[LOC_SIZE], int f[LOC_SIZE]) override;
  
 public:

  Queen(std::string const unicode_macro);

  ~Queen(){}; 
}; 

#endif
