MAKE = g++
INCLUDE = include
SRC = main.cpp src/*.cpp
LIB = -lsfml-graphics -lsfml-window -lsfml-system

all:
	$(MAKE) $(SRC) -I $(INCLUDE) $(LIB) -o chess

test:
	$(MAKE) test.cpp src/*.cpp -I $(INCLUDE) $(LIB) -o chess