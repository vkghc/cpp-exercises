OBJ = ChessMain.o ChessBoard.o Piece.o Pawn.o Knight.o Bishop.o Rook.o Queen.o King.o
EXE = chess
CXX = g++
CXXFLAGS = -g -Wall -MMD

$(EXE): $(OBJ)
	$(CXX) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

-include $(OBJ:.o=.d)

clean:
	rm -rf $(OBJ) $(EXE) $(OBJ:.o=.d)

.PHONY: clean
