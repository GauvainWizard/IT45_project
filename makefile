SRC_DIR := src
OBJ_DIR := obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
CXX := g++
LDFLAGS := -lm
CPPFLAGS := \
	-L/lib \
	-L/usr/lib \
	-L/usr/local/lib
CXXFLAGS := -c -g -Wall -pedantic -std=c++11 -Ofast 

main: $(OBJ_FILES)
	$(CXX) $(LDFLAGS) $(CPPFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean : 
	rm -f *.o *~

clear : 
	make clean;
	rm -f main