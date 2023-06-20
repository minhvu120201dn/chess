MAKE = g++
INCLUDE = include
SRC = main.cpp src/*.cpp
LIB = -lsfml-graphics -lsfml-window -lsfml-system 

all:
	$(MAKE) $(SRC) -I$(INCLUDE) -I/opt/homebrew/Cellar/sfml/2.5.1_2/include $(LIB) -o chess -L/opt/homebrew/Cellar/sfml/2.5.1_2/lib

test:
	$(MAKE) test.cpp src/*.cpp -I $(INCLUDE) $(LIB) -o chess