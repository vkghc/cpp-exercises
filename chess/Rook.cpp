#include "Rook.h"

using namespace std;

// private members
bool Rook::checkPattern(int s[LOC_SIZE], int f[LOC_SIZE]){

  if(s[_RANK] == f[_RANK] || s[_FILE] == f[_FILE])
    return 1; 
  
  return 0; 
}

bool Rook::checkNoObstacleOnWay(int s[LOC_SIZE], int f[LOC_SIZE]){

  // case 1: moving up a file
  if(s[_FILE] == f[_FILE] && s[_RANK] < f[_RANK]){
    int c = s[_FILE];
    
    for(int i = s[_RANK]+1; i < f[_RANK]; i++)
      if(board_copy_[i][c] != nullptr)
	return 0; 
  }

  // case 2: moving down a file
  if(s[_FILE] == f[_FILE] && s[_RANK] > f[_RANK]){
    int c = s[_FILE];
    
    for(int i = s[_RANK]-1; i > f[_RANK]; i--)
      if(board_copy_[i][c] != nullptr)
	return 0; 
  }


  // case 3: moving right a rank
  if(s[_RANK] == f[_RANK] && s[_FILE] < f[_FILE]){
    int c = s[_RANK];
    
    for(int i = s[_FILE]+1; i < f[_FILE]; i++)
      if(board_copy_[c][i] != nullptr)
	return 0; 
  }

  // case 4: moving left a rank
  if(s[_RANK] == f[_RANK] && s[_FILE] > f[_FILE]){
    int c = s[_RANK];
    
    for(int i = s[_FILE]-1; i > f[_FILE]; i--)
      if(board_copy_[c][i] != nullptr)
	return 0; 
  }
  
  return 1; 
}

// public members
Rook::Rook(string const unicode_macro) :
  Piece(unicode_macro) {
  name_ = "Rook";
  type_ = RookType; 
}
