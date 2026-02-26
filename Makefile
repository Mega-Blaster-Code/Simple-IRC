CC = gcc
FLAGS = -std=c11 -Wall -Wextra -O0 -g -Werror -Wno-unused-function -Wno-unused-variable  -Wno-unused-parameter

SRC_PATH = src
RAY_PATH = src
OBJ_PATH = objs
LIB_PATH = lib

SRC := $(wildcard $(SRC_PATH)/*.c) $(wildcard $(RAY_PATH)/*.c)
OBJ := $(patsubst $(SRC_PATH)/%.c,$(OBJ_PATH)/%.o,$(SRC))

TARGET = main

LIBS = -L$(LIB_PATH)

all: $(OBJ_PATH) $(TARGET) run

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)

$(TARGET): $(OBJ)
	@$(CC) $^ -o $@ $(LIBS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_PATH)/*.o $(TARGET)

run:
	@./$(TARGET)
