/* Header for piece base class */
#include "macros.h"
#include <iostream>
#include <string>

#ifndef PIECE_H
#define PIECE_H

enum Type{None, PawnType, KnightType, BishopType, RookType,
	  QueenType, KingType};

enum Color{B, W, N};

class Piece{
 protected:
  
  std::string name_;
  std::string symbol_;
  Type type_; 
  Color color_;
  Color enemy_color_; 
  Piece* board_copy_[8][8];
  int times_moved_;
  
  virtual bool checkPattern(int s[LOC_SIZE], int f[LOC_SIZE])=0;
  
  virtual bool checkNoObstacleOnWay(int s[LOC_SIZE], int f[LOC_SIZE])=0;
  
  bool checkDestination(int s[LOC_SIZE], int f[LOC_SIZE]);
  
 public:
  
  Piece(std::string const unicode_macro);

  std::string getName() const;
  
  std::string getSymbol() const;
  
  Color getColor() const;
  
  Type getType() const;
  
  void setSquareCopy(int s[LOC_SIZE], Piece* t);
  
  bool checkMove(int s[LOC_SIZE], int f[LOC_SIZE]);
  
  bool checkIsChecking(int f[LOC_SIZE]);

  int getTimesMoved(); 

  void incrementTimesMoved();

  virtual int getSpecialFeature();

  virtual void setCheckedFlag(bool c);

  virtual void setCastleSide(int s);
  
  virtual bool getCastlingRequest(); 
  
  virtual ~Piece(){}; 
};

#endif
