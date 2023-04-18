CC        := gcc
C_FLAGS := -Wall -Wextra -pedantic -O2 # -Wall -Wextra -std=c11 -g
C_DEBUG_FLAGS := -g


BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:=-lm -lcurses
EXECUTABLE	:= main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

debug: C_FLAGS += $(C_DEBUG_FLAGS)
debug: $(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(shell find ./$(SRC) -name '*.c')
	$(CC) $(C_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)
	$(MAKE) copyassets

clean:
	-rm $(BIN)/*
	-rm -r $(BIN)/assets

copyassets: 
	mkdir -p $(BIN)/assets
	rsync -av ./assets $(BIN) --exclude=.gitkeep
