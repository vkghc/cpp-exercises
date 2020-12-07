#include "Queen.h"

using namespace std;

// private members
bool Queen::checkPattern(int s[LOC_SIZE], int f[LOC_SIZE]){

  if(Rook::checkPattern(s, f) || Bishop::checkPattern(s, f))
    return 1; 
  
  return 0; 
}

bool Queen::checkNoObstacleOnWay(int s[LOC_SIZE], int f[LOC_SIZE]){

  bool test; 

  if(Rook::checkPattern(s, f)){

    test = Rook::checkNoObstacleOnWay(s, f);
    return test;
    
  }else if(Bishop::checkPattern(s, f)){
    
    test = Bishop::checkNoObstacleOnWay(s, f);
    return test; 
  }
  
  return 0; // should never get here, but just in case 
}

// public members
Queen::Queen(string const unicode_macro) :
  Piece(unicode_macro),
  Rook(unicode_macro),
  Bishop(unicode_macro) {

  name_ = "Queen";
  type_ = QueenType; 
}
