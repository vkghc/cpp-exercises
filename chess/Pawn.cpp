#include "Pawn.h"

using namespace std;

// private member functions
bool Pawn::checkPattern(int s[LOC_SIZE], int f[LOC_SIZE]){ 
  
  if(color_ == W){

    takes_in_passing_ = NO_EN_PASSANT;
    
    // check en-passant attack
    if(!board_copy_[f[_RANK]][f[_FILE]])
      if(s[_RANK] + 1 == f[_RANK] &&
	 (s[_FILE] + 1 == f[_FILE] ||
	  s[_FILE] - 1 == f[_FILE]) &&
	 board_copy_[s[_RANK]][f[_FILE]] &&
	 board_copy_[s[_RANK]][f[_FILE]]->getType() == PawnType &&
	 board_copy_[s[_RANK]][f[_FILE]]->getTimesMoved() == 1){

	takes_in_passing_ =  EN_PASSANT; 
	return 1;
      }
    
    // check regular attack
    if(board_copy_[f[_RANK]][f[_FILE]])
      if(s[_RANK]+1 == f[_RANK] &&
	 (s[_FILE]+1 == f[_FILE] ||
	  s[_FILE]-1 == f[_FILE]))
	return 1;

    // if not attacking, only straight
    if(s[_FILE] == f[_FILE]){
      if(times_moved_ == 0 && s[_RANK]+2 == f[_RANK])
	return 1;
    
      else if(s[_RANK]+1 == f[_RANK])
	return 1;
    }
    
  }
  
  if(color_ == B){

    // check en-passant attack
    if(!board_copy_[f[_RANK]][f[_FILE]])
      if(s[_RANK] - 1 == f[_RANK] &&
	 (s[_FILE] + 1 == f[_FILE] ||
	  s[_FILE] - 1 == f[_FILE]) &&
	 board_copy_[s[_RANK]][f[_FILE]] &&
	 board_copy_[s[_RANK]][f[_FILE]]->getType() == PawnType &&
	 board_copy_[s[_RANK]][f[_FILE]]->getTimesMoved() == 1){
	
	takes_in_passing_ =  EN_PASSANT; 
	return 1;
      }
    
      
    // check regular attack
    if(board_copy_[f[_RANK]][f[_FILE]])
      if(s[_RANK]-1 == f[_RANK] &&
	 (s[_FILE]+1 == f[_FILE] ||
	  s[_FILE]-1 == f[_FILE]))
	return 1;

    // if not attacking, only straight
    if(s[_FILE] == f[_FILE]){
      if(times_moved_ == 0 && s[_RANK]-2 == f[_RANK])
	return 1;
      
      else if(s[_RANK]-1 == f[_RANK])
	return 1;
    }
  } 
  return 0; 
}


bool Pawn::checkNoObstacleOnWay(int s[LOC_SIZE], int f[LOC_SIZE]){
  
  // if attacking, allowed sideways
  if(board_copy_[f[_RANK]][f[_FILE]])
    if(s[_RANK]-1 == f[_RANK] &&
       (s[_FILE]+1 == f[_FILE] ||
	s[_FILE]-1 == f[_FILE]))
      return 1;
  
  if(color_ == W){

    if(times_moved_ == 0 &&
     s[_RANK]+2 == f[_RANK])
    if(board_copy_[s[_RANK]+1][s[_FILE]] == nullptr)
      return 1;

  if( s[_RANK]+1 == f[_RANK])
    if(board_copy_[s[_RANK]+1][s[_FILE]] == nullptr)
      return 1;
  }

  if(color_ == B){

    if(times_moved_ == 0 &&
     s[_RANK]-2 == f[_RANK])
    if(board_copy_[s[_RANK]-1][s[_FILE]] == nullptr)
      return 1;

  if( s[_RANK]-1 == f[_RANK])
    if(board_copy_[s[_RANK]-1][s[_FILE]] == nullptr)
      return 1;
  }
  
  return 0; 
}

// end private members

// public members
Pawn::Pawn(string const unicode_macro) :
  Piece(unicode_macro),
  takes_in_passing_(0) {

  name_ = "Pawn";
  type_ = PawnType; 
}

int Pawn::getSpecialFeature(){
  return takes_in_passing_; 
}
