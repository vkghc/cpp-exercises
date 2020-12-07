#include "Knight.h"

using namespace std;

// private members
bool Knight::checkPattern(int s[LOC_SIZE], int f[LOC_SIZE]){

  if(s[_FILE]+1 == f[_FILE] ||
     s[_FILE]-1 == f[_FILE])
    if(s[_RANK]+2 == f[_RANK] ||
       s[_RANK]-2 == f[_RANK])
      return 1;
  
  if(s[_FILE]+2 == f[_FILE] ||
     s[_FILE]-2 == f[_FILE])
    if(s[_RANK]+1 == f[_RANK] ||
       s[_RANK]-1 == f[_RANK])
      return 1; 
  
  return 0; 
}


bool Knight::checkNoObstacleOnWay(int s[LOC_SIZE], int f[LOC_SIZE]){
  
  return 1; 
}

// public members
Knight::Knight(string const unicode_macro) :
  Piece(unicode_macro) {
  name_ = "Knight";
  type_ = KnightType; 
}
