SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

BIN_DIR=bin
DEP_DIR=deps
BIN=$(BIN_DIR)/fractalview

CPPC=clang++
SHARED_ARGS=-std=c++1y -Ofast -g -ggdb -m64 -mmmx -msse -msse2 -m3dnow -fexceptions -Wall -Wpedantic -DDEBUG -DNO_SHOCKWAVE_DEBUG -DVSYNC
COMPILE_ARGS=-c $(SHARED_ARGS)
LINK_ARGS=$(SHARED_ARGS)
LIBS=-lglfw -lGL -lGLEW
INCLUDE=-I.

$(BIN): $(OBJECTS) Makefile $(BIN_DIR)
	$(CPPC) $(LINK_ARGS) $(OBJECTS) $(INCLUDE) $(LIBS) -o $(BIN)

$(DEP_DIR):
	mkdir -p $(DEP_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

%.o: %.cpp Makefile $(DEP_DIR)
	$(CPPC) $(COMPILE_ARGS) $< -MMD -MF $(DEP_DIR)/$(patsubst %.o,%.md,$@)

run: $(BIN)
	./$(BIN)

primusrun: $(BIN)
	primusrun ./$(BIN)

debug: $(BIN)
	gdb ./$(BIN)

valg: $(BIN)
	valgrind --leak-check=full $(BIN)

include $(wildcard $(DEP_DIR)/*.md)

clean:
	rm $(BIN) $(OBJECTS); true
