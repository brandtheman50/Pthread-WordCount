CXX = g++
CXXFLAGS = -std=c++11
LDFLAGS = -pthread

SRC = wordcount.cpp
OBJ = $(SRC:.cpp=.cpp)
EXEC = wordcount

make: $(EXEC)
$(EXEC): $(OBJ)
		$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)


