#include "Piece.h"

using namespace std;

// private members

bool Piece::checkDestination(int s[LOC_SIZE], int f[LOC_SIZE]){

  Piece* b = board_copy_[f[_RANK]][f[_FILE]];
  
  Color destination = ( b== nullptr) ? N : b->getColor();
  
  // making use of bit-wise comparison here... a bit ugly
  if(color_ != destination)
    return 1;
  
  return 0; 
}
// end private members

// public members
Piece::Piece(string const unicode_macro) :
  name_("Piece"),
  symbol_(unicode_macro),
  type_(None),
  times_moved_(0) {

  // initialise colors
  if(unicode_macro == WK ||
     unicode_macro == WQ ||
     unicode_macro == WR ||
     unicode_macro == WB ||
     unicode_macro == WN ||
     unicode_macro == WP){
    color_ = W;
    enemy_color_= B;
  }
  else{
    color_ = B;
    enemy_color_ = W; 
  }

  for(int i = 0; i < FIN; i++)
    for(int j = 0; j < FIN; j++)
      board_copy_[i][j] = nullptr;
  
}

string Piece::getName() const{
  return name_; 
}

string Piece::getSymbol() const{
  return symbol_; 
}

Color Piece::getColor() const{
  return color_; 
}

Type Piece::getType() const{
  return type_; 
}

void Piece::setSquareCopy(int s[LOC_SIZE], Piece* t){
  board_copy_[s[_RANK]][s[_FILE]] = t; 
}

bool Piece::checkMove(int s[LOC_SIZE], int f[LOC_SIZE]){

  // can't move a piece on itself
  if(s[_RANK] == f[_RANK] &&
     s[_FILE] == f[_FILE])
    return 0; 

  if(!checkPattern(s, f))
    return 0;
  
  if(!checkNoObstacleOnWay(s , f))
    return 0;
  
  if(!checkDestination(s, f))
    return 0;
  
  return 1; 
}

bool Piece::checkIsChecking(int f[LOC_SIZE]){

  int opp_king_loc[LOC_SIZE];
  
  for(int i =0; i < FIN; i++)
    for(int j =0; j < FIN; j++)
      if(board_copy_[i][j] != nullptr)
	if( board_copy_[i][j]->getColor() == enemy_color_ &&
	    board_copy_[i][j]->getType() == KingType){
	
	  opp_king_loc[_RANK] = i;
	  opp_king_loc[_FILE] = j; 
	}
  
  if(checkMove(f, opp_king_loc))
    return 1;
  
  return  0;
}

int Piece::getTimesMoved(){
  return times_moved_; 
}

void Piece::incrementTimesMoved(){
  times_moved_++; 
}


// placeholders for virtual functions which cannot be pure virutal
int Piece::getSpecialFeature(){return NO_CASTLE;} // castle and enpassant
bool Piece::getCastlingRequest(){return 0;}
void Piece::setCheckedFlag(bool c){return; }
void Piece::setCastleSide(int s){return; }
// end public members
