OBJ = main.o enigma.o
EXE = enigma
CXX = g++
CXXFLAGS = -Wall -g -MMD

$(EXE): $(OBJ)
	$(CXX) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

-include $(OBJ:.o=.d)

clean: 
	rm -f $(OBJ) $(EXE) $(OBJ:.o=.d)

.PHONY: clean
