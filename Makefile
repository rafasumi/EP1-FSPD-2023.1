CC = gcc
LIBS = -lm -lpthread

INCLUDE_FOLDER = ./include/
OBJ_FOLDER = ./obj/
SRC_FOLDER = ./src/

MAIN = main
TARGET = ex1
SRC = $(wildcard $(SRC_FOLDER)*.c)
OBJ = $(patsubst $(SRC_FOLDER)%.c, $(OBJ_FOLDER)%.o, $(SRC))

$(OBJ_FOLDER)%.o: $(SRC_FOLDER)%.c
	@mkdir -p $(OBJ_FOLDER)
	$(CC) -c $< -o $@ -I$(INCLUDE_FOLDER) $(LIBS)

build: $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LIBS)

clean:
	@rm -rf $(OBJ_FOLDER) $(TARGET)
