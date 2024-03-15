NAME_EXE=server
CMP=g++

PREF_SRC=../

SOURCE=$(wildcard $(PREF_SRC)*.cpp)
OBJ=$(patsubst $(PREF_SRC)%.cpp, %.o, $(SOURCE))

$(NAME_EXE): $(OBJ)
		$(CMP) $(OBJ) -o $(NAME_EXE)
	
%.o: $(PREF_SRC)%.cpp
		$(CMP) -c $< -o $@
	
clean: 
		rm *.o
