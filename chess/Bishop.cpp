#include "Bishop.h"
#include <cmath>

using namespace std;

// private members
bool Bishop::checkPattern(int s[LOC_SIZE], int f[LOC_SIZE]){

  if(f[_FILE]- s[_FILE] == f[_RANK]-s[_RANK] ||
     f[_FILE]- s[_FILE] == -(f[_RANK]-s[_RANK]))
    return 1; 
  
  return 0; 
}

bool Bishop::checkNoObstacleOnWay(int s[LOC_SIZE], int f[LOC_SIZE]){

  int dif = abs(f[_RANK] - s[_RANK]);
  
  // case 1: going up and right
  if(s[_RANK] < f[_RANK] && s[_FILE] < f[_FILE]){
    for(int i = 1; i < dif; i++)
      if(board_copy_[s[_RANK] + i][s[_FILE] + i] != nullptr)
	return 0; 
  }

  // case 2: going up and left
  if(s[_RANK] < f[_RANK] && s[_FILE] > f[_FILE]){
    for(int i = 1; i < dif; i++)
      if(board_copy_[s[_RANK] + i][s[_FILE] - i] != nullptr)
	return 0; 
  }

  // case 3: going down and right
  if(s[_RANK] > f[_RANK] && s[_FILE] < f[_FILE]){
    for(int i = 1; i < dif; i++)
      if(board_copy_[s[_RANK] - i][s[_FILE] + i] != nullptr)
	return 0; 
  }

  // case 4: going down and left
  if(s[_RANK] > f[_RANK] && s[_FILE] > f[_FILE]){
    for(int i = 1; i < dif; i++)
      if(board_copy_[s[_RANK] - i][s[_FILE] - i] != nullptr)
	return 0; 
  }
  
  return 1; 
}

// public members
Bishop::Bishop(string const unicode_macro) :
  Piece(unicode_macro) {
  name_ = "Bishop";
  type_ = BishopType; 
}
