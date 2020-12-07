#include "ChessBoard.h"

using namespace std;

// friend functions to help printing output
ostream& operator<<(ostream& o, const Piece* p){

  string color = (p->getColor() == W) ? "White" : "Black";

  return o << color << "'s " << p->getName();  
}

// private members
void ChessBoard::initialiseNewBoard(){
  for(int i = 0; i < FIN; i++)
    for(int j =0; j < FIN; j++)
      {
	in_turn_ = W;
	move_num_ = 1;
	en_passant_ = NO_EN_PASSANT; 
	in_check_ = N;
	in_mate_ = N;
	in_stalemate_ = N; 
	
	// white pieces
	if(i == 0)
	  {
	    if(j == 0 || j == 7)
	      current_board_[i][j] = new Rook(WR);
	    if(j == 1 || j == 6)
	      current_board_[i][j] = new Knight(WN);
	    if(j == 2 || j == 5)
	      current_board_[i][j] = new Bishop(WB);
	    if(j == 3)
	      current_board_[i][j] = new Queen(WQ);
	    if(j == 4){
	      current_board_[i][j] = new King(WK);
	      white_king_ = current_board_[i][j];
	    }
	  }
	else if(i == 1)
	  current_board_[i][j] = new Pawn(WP);
	
	// black pieces
	else if(i == 7)
	  {
	    if(j == 0 || j == 7)
	      current_board_[i][j] = new Rook(BR);
	    if(j == 1 || j == 6)
	      current_board_[i][j] = new Knight(BN);
	    if(j == 2 || j == 5)
	      current_board_[i][j] = new Bishop(BB);
	    if(j == 3)
	      current_board_[i][j] = new Queen(BQ);
	    if(j == 4){
	      current_board_[i][j] = new King(BK);
	      black_king_ = current_board_[i][j];
	    }
	  }
	else if(i == 6)
	  current_board_[i][j] = new Pawn(BP);
	else
	  current_board_[i][j] = nullptr;
      }
  cout << "A new chess game is started!" << endl;
} 

void ChessBoard::destroyOldBoard(){

  for(int i =0; i < FIN; i++)
    for(int j =0; j < FIN; j++)
      if(current_board_[i][j]){
	delete current_board_[i][j];
	current_board_[i][j] = nullptr; 
      }
}

// decides what to print based on the game flags (check, mate, etc..)
void ChessBoard::printOutput(int event,
			     string const start, string const fin,
			     Piece* p){

  string color = (in_check_ == W) ? "White" : "Black";
  int move = ((move_num_ % 2) == 0) ? move_num_/2 : (move_num_+1)/2;
  
  switch(event){
    
  case CONTINUE:
    
    cout << "Move " << move << ": " << p
	 << " moves from " << start << " to " << fin << endl;
    
    if(p->getSpecialFeature() == QUEEN_SIDE)
      cout << "Queen-side castle" << endl;
    
    if(p->getSpecialFeature() == KING_SIDE)
      cout << "Kingside castle" << endl;

    if(en_passant_ == EN_PASSANT)
      cout << "Takes en-passant" << endl;
    
    if(in_check_ != N && in_mate_ == N)
      cout << color <<  " is in check" << endl;

    if(in_check_ != N && in_mate_ != N)
      cout << "Game over: " << color <<  " is in checkmate" << endl;

    if(in_stalemate_ != N)
      cout << "Game is a draw: " << color <<  " is in stalemate" << endl;
    
    break;

  case INVALID_COORDINATES:
    cout << "Error: " << start << " or " << fin
	 << " are invalid coordinates" << endl; 
    break;

  case DEAL_WITH_CHECK:
    cout << "Error: " << p
	 << " move from " <<  start << " to " << fin
	 << " will leave King in check" << endl; 
    break;

  case INVALID_MOVE:
    cout << "Error: " << p
	 << " cannot move from " <<  start << " to " << fin << endl; 
    break;

  case NO_PIECE:
    cout << "Error: There is no piece on " << start << endl;
    break;

  case WRONG_TURN:
    cout << "Error: It is not " << color << "'s turn to move" << endl;
  }
}

void ChessBoard::passBoardCopy(int s[LOC_SIZE]){

  for(int i =0; i<FIN; i++)
    for(int j =0; j<FIN; j++){

      int loc[LOC_SIZE] = {i, j};       

      // if empty 
      if(current_board_[loc[_RANK]][loc[_FILE]] == nullptr){

	Piece* t = nullptr; 
	current_board_[s[_RANK]][s[_FILE]]->setSquareCopy(loc, t);
      }
      else{
        Piece* t = current_board_[loc[_RANK]][loc[_FILE]]; 
	current_board_[s[_RANK]][s[_FILE]]->setSquareCopy(loc, t);
      } 
    } 

}

// are the coordinates submitted valid?
bool ChessBoard::checkCoordinates(string const start, string const fin){

  int si = 0, fi = 0; 
  while(start[si] != '\0')
    si++;
  while(fin[fi] != '\0')
    fi++;
  
  if(si != 2 || fi !=2)
    return 0; 
  
  if(start[_RANK] < ASC_A || start[_RANK] > ASC_H ||
     fin[_RANK] < ASC_A || fin[_RANK] > ASC_H)
    return 0 ;

  return 1; 
}

// convert "A1" notation to the coordinates for the board[8][8] array
void ChessBoard::convertCoord(string const location, int loc[2]){

  // switch order of coordinates:
  // ture coords are {row, column}; "A1" specifies {column A, row 1}
  loc[_FILE] = location[_RANK] - ASC_A;
  loc[_RANK] = location[_FILE] - ASC_1;
}

// once a move has been checked, make it 
void ChessBoard::makeMove(int s[LOC_SIZE], int f[LOC_SIZE]){

  // delete taken piece
  if(current_board_[f[_RANK]][f[_FILE]]){
    
    delete current_board_[f[_RANK]][f[_FILE]];

    current_board_[f[_RANK]][f[_FILE]] = nullptr; 
  }
  
  // make move by shifting the board pointers
  current_board_[f[_RANK]][f[_FILE]] = current_board_[s[_RANK]][s[_FILE]];
  current_board_[s[_RANK]][s[_FILE]] = nullptr;
  current_board_[f[_RANK]][f[_FILE]]->incrementTimesMoved();

  // if the move was castle, also move corresponding rook;
  int castle = current_board_[f[_RANK]][f[_FILE]]->getSpecialFeature();

  if(castle == KING_SIDE){
    
    current_board_[s[_RANK]][K_ROOK_CASTLE] = current_board_[s[_RANK]][7];

    current_board_[s[_RANK]][K_ROOK_START] = nullptr; 
  }
  
  if(castle == QUEEN_SIDE){
    
    current_board_[s[_RANK]][Q_ROOK_CASTLE] = current_board_[s[_RANK]][0];

    current_board_[s[_RANK]][Q_ROOK_START] = nullptr; 
  }

  // SPECIAL MOVES END

}


ChessBoard::ChessBoard() {
  
  initialiseNewBoard(); 
  
}

void ChessBoard::submitMove(string start, string fin){

  // useful pointers to keep track of kings
  Piece* own_king = (in_turn_ == W) ?  white_king_ : black_king_;
  Piece* enemy_king = (in_turn_ == B) ?  white_king_ : black_king_;
    
  // if invalid coordinates
  if(!checkCoordinates(start, fin)){
    printOutput(INVALID_COORDINATES, start, fin, nullptr);
    return; 
  }

  int s[LOC_SIZE];
  convertCoord(start, s);

  int f[LOC_SIZE];
  convertCoord(fin, f);

  // helpful pointers to save notation
  Piece* p = current_board_[s[_RANK]][s[_FILE]]; 

  // if trying to move from empty square
  if(!p){
    printOutput(NO_PIECE, start, fin, nullptr); 
    return; 
  }

  // if trying to move opposite color piece
  if(p->getColor() != in_turn_){
    printOutput(WRONG_TURN, start, fin, p); 
    return; 
  }
  
  // pass copy of board to all pieces
  passBoardCopy(s); 
    
  
  // check if move is legal for moving piece to make
  if(!p->checkMove(s, f) ){
    printOutput(INVALID_MOVE, start, fin, p); 
    return;
  }

  // if move is castle, check if intermediate checks
  if(checkCastleRequest(own_king, s, f) == BAD_CASTLE){
    printOutput(INVALID_MOVE, start, fin, p); 
    return;
  }

  // if move is en-passant, set flag
  if(p->getSpecialFeature() == EN_PASSANT)
    en_passant_ = EN_PASSANT;
  else
    en_passant_ = NO_EN_PASSANT; 
  
  // you cannot open yourself to check
  if(checkOwnKingSafety(s,f) ==0 ){
    printOutput(DEAL_WITH_CHECK, start, fin, p); 
    return;
  }
    
  // make move
  makeMove(s, f);
  
  // set flag if opponent  in check
  setInCheckFlag(enemy_king);

  
  // change turn temoporarily (a bit ugly)
  in_turn_ = (in_turn_ == W) ? B : W;

  // update own king as switched side
  own_king = (in_turn_ == W) ?  white_king_ : black_king_;
  
  // check if new side in checkmate 
  if(in_check_ != N)
    if(checkAnyPossibleMoves(in_turn_, own_king) == 0)
      in_mate_ = own_king->getColor(); 
  
  
  // check if new side in stalemate
  if(in_check_ == N)
    if(checkAnyPossibleMoves(in_turn_, own_king) == 0)
      in_stalemate_ = own_king->getColor(); 
  
  // some output
  printOutput(CONTINUE, start, fin, p);
  move_num_++;
  
}

// check if your own king is safe given the move you just made
bool ChessBoard::checkOwnKingSafety(int s[LOC_SIZE], int f[LOC_SIZE]){

  // temporarily make the move you wanted to make
  Piece* temp_start_ptr = current_board_[s[_RANK]][s[_FILE]]; 
  Piece* temp_fin_ptr = current_board_[f[_RANK]][f[_FILE]];
  current_board_[f[_RANK]][f[_FILE]] = temp_start_ptr;
  current_board_[s[_RANK]][s[_FILE]] = nullptr;

  
  // find the location of your  king... quite ugly
  int king_loc[LOC_SIZE];
  
  for(int i =0; i < FIN; i++)
    for(int j =0; j < FIN; j++)
      
      if(current_board_[i][j] != nullptr)
	if( current_board_[i][j]->getColor() == in_turn_ &&
	    current_board_[i][j]->getType() == KingType){
	  
	  king_loc[_RANK] = i;

	  king_loc[_FILE] = j;

	  break; 
	}
  
  // find if any of opponents pieces are checking you
  for(int i =0; i <FIN; i++)
    for(int j =0; j <FIN; j++)
      if(current_board_[i][j] &&
	 current_board_[i][j]->getColor() != in_turn_){
	
	int piece_loc[2] = {i, j};

	passBoardCopy(piece_loc); 

	if(current_board_[i][j]->checkMove(piece_loc, king_loc)){
	  
	  // reverse move
	  current_board_[s[_RANK]][s[_FILE]] = temp_start_ptr;
	  current_board_[f[_RANK]][f[_FILE]] = temp_fin_ptr;

	  // you can't make the move!
	  return 0;
	}
      }

  // reverse move
  current_board_[s[_RANK]][s[_FILE]] = temp_start_ptr;
  current_board_[f[_RANK]][f[_FILE]] = temp_fin_ptr;

  // your king is safe, you can make the move
  return 1; 
}

// if any of your pieces are checking the enemy king, raise flag
void ChessBoard::setInCheckFlag(Piece* enemy_king){
  
  // find all your pieces
  for(int i =0; i <FIN; i++)
    for(int j =0; j <FIN; j++)
      
      if(current_board_[i][j])
	if(current_board_[i][j]->getColor() == in_turn_){

	int piece_loc[2] = {i, j};
	passBoardCopy(piece_loc);

	// see if they  attack the enemy king
	if(current_board_[i][j]->checkIsChecking(piece_loc)){  

	  // if yes, enemy is in check
	  in_check_ = (in_turn_ == W) ? B : W;
	  enemy_king->setCheckedFlag(1); 

	  return; 
	}
      } 

  // if not, enemy is not not in check
  enemy_king->setCheckedFlag(0); 
  in_check_ = N;
  
  return; 
}

// if the king wishes to castle, this function checks
// whether the king will be checked while "in transit"
int ChessBoard::checkCastleRequest(Piece* own_king,
				int s[LOC_SIZE], int f[LOC_SIZE]){

  if(!own_king->getCastlingRequest())
    return NO_CASTLE;

  
  // king-side castle
  if(s[_FILE]+2 == f[_FILE]){
    
    int medpos[LOC_SIZE] = {s[_RANK], s[_FILE]+1 };
    
    if(!checkOwnKingSafety(s, medpos))
      return BAD_CASTLE;

    own_king->setCastleSide(KING_SIDE); 
    return KING_SIDE; 
    
  }

  // queen-side castle
  if(s[_FILE]-2 == f[_FILE]){

    int medpos[LOC_SIZE] = {s[_RANK], s[_FILE]-1 };

    if(!checkOwnKingSafety(s, medpos))
      return BAD_CASTLE;

    own_king->setCastleSide(QUEEN_SIDE); 
    return QUEEN_SIDE; 
    
  }


  return NO_CASTLE; //not needed, but just in case
} 

// when your turn starts, this function checks that you have any
// legal moves left. If you don't the game ends in checkmate or stalemate
bool ChessBoard::checkAnyPossibleMoves(Color color, Piece* king){

  // cycle through evey square in the board
  for(int i = 0; i < FIN; i++)
    for(int j = 0; j < FIN; j++)

      // find all your own pieces
      if(current_board_[i][j] != nullptr)
	if(current_board_[i][j]->getColor() == color){

	  // try to move every piece you have anywhere on the board
	  for(int k = 0; k < FIN; k++)
	    for(int l =0; l < FIN; l++){
	      
	      int a[LOC_SIZE] = {i, j};

	      int b[LOC_SIZE] = {k, l};

	      passBoardCopy(a);

	      // if the move is legal and leaves your king safe,
	      // you have moves left so you can proceed to
	      // your own turn
	      if(current_board_[i][j]->checkMove(a, b) &&
		 checkOwnKingSafety(a, b))
		return 1;
	    }
	}
  
  return 0; 
}

void ChessBoard::resetBoard(){
  
  destroyOldBoard(); 
 
  initialiseNewBoard();
  
}

ChessBoard::~ChessBoard(){
  
  destroyOldBoard(); 
}
