COMPILER=g++
FLAGS=-std=c++11
MAIN=src/dfxam.cc
TEST=test/basic.cc
SRCS= $(wildcard src/repl/*.cc) $(wildcard src/ast/*.cc)
INCLUDES=include/

all:
	$(COMPILER) $(FLAGS) $(MAIN) $(SRCS) -I $(INCLUDES)

test:
	$(COMPILER) $(FLAGS) $(TEST) $(SRCS) -I $(INCLUDES)
