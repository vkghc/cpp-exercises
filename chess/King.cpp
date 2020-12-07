#include "King.h"

using namespace std;

// private members
bool King::checkPattern(int s[LOC_SIZE], int f[LOC_SIZE]){

  if(s[_RANK] + 1 == f[_RANK] ||
     s[_RANK] - 1 == f[_RANK] ||
     s[_RANK] == f[_RANK]) 
    if(s[_FILE] + 1 == f[_FILE] ||
       s[_FILE] - 1 == f[_FILE])
      return 1;

  // also allow castle pattern in specific circumstances
  if(times_moved_ == 0 &&
     s[_RANK] == f[_RANK] &&
     ( s[_FILE] + 2 == f[_FILE] ||
       s[_FILE] - 2 == f[_FILE] ) &&
     is_checked_ == 0){

    if(s[_FILE] + 2 == f[_FILE])
      castling_request_ = KING_SIDE;
    else
      castling_request_ = QUEEN_SIDE; 
    
    return 1; 
  }

  castling_request_ = 0; 
  return 0; 
}

// a king can't move right next to another king
bool King::checkNoObstacleOnWay(int s[LOC_SIZE], int f[LOC_SIZE]){
  
  int c = s[_RANK]; 

  if( s[_FILE]+2 == f[_FILE] )
    for(int i = s[_FILE]+1; i < FIN-1; i++)
      if(board_copy_[c][i] != nullptr)
	return 0;

  if( s[_FILE]-2 == f[_FILE] )
    for(int i = s[_FILE]-1; i > 0; i--)
      if(board_copy_[c][i] != nullptr)
	return 0;

  return 1;
}

// public members
King::King(string const unicode_macro) :
  Piece(unicode_macro),
  is_castled_(NO_CASTLE),
  is_checked_(0),
  castling_request_(0)  {
  
  name_ = "King";
  type_ = KingType;
}

int King::getSpecialFeature(){
  return is_castled_; 
}

void King::setCheckedFlag(bool c){
  is_checked_ = c; 
}

bool King::getCastlingRequest(){
  return castling_request_; 
}

void King::setCastleSide(int s){
  is_castled_ = s; 
}
