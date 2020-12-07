#include "Piece.h"

#ifndef KING_H
#define KING_H

class King : public Piece {
  
 protected:

  int is_castled_;

  bool is_checked_;

  bool castling_request_; 
  
  bool checkPattern(int s[LOC_SIZE], int f[LOC_SIZE]) override;

  bool checkNoObstacleOnWay(int s[LOC_SIZE], int f[LOC_SIZE]) override;
  
 public:

  King(std::string const unicode_macro);

  int getSpecialFeature() override;

  bool getCastlingRequest() override; 

  void setCheckedFlag(bool c) override;

  void setCastleSide(int s) override;

  ~King(){}; 
}; 

#endif
