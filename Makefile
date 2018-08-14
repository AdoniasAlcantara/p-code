# Compiler
CP = gcc
override CFLAGS += -Wall -c

# Directories
BIN = ./bin
OBJ = ./build
INC = ./include
SRC = ./src

# App name
APP ?= main

.PHONY: all dirs clean
.PRECIOUS: $(OBJ)/%.o

all: dirs $(BIN)/$(APP)

$(BIN)/%: $(patsubst $(SRC)%.c,$(OBJ)%.o,$(wildcard $(SRC)/*.c))
	$(CP) -o $@ $^

$(OBJ)/%.o: $(SRC)/%.c
	$(CP) $(CFLAGS) -o $@ $< -I $(INC)

dirs: $(BIN) $(OBJ)

$(BIN): 
	@mkdir $(BIN)

$(OBJ): 
	@mkdir $(OBJ)

clean:
	@rm -f $(BIN)/* $(OBJ)/*
