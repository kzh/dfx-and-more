COMPILER=g++
FLAGS=-std=c++11
MAIN=
TEST=test/basic.cc
SRCS=$(wildcard src/*.cc) $(wildcard src/ast/*.cc)
INCLUDES=include/

all:
	$(COMPILER) $(FLAGS) $(TEST) $(SRCS) -I $(INCLUDES)
