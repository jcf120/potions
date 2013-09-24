CC=g++
CFLAGS=-std=c++11 -c -Wall -Werror
OBJS=main.o Instructor.o Alchemist.o Discovery.o Ingredient.o StatusEffect.o
OBJ_DIR=obj/
SRC_DIR=src/

all: potions

potions: $(OBJ_DIR)main.o $(OBJ_DIR)Instructor.o $(OBJ_DIR)Alchemist.o \
		 $(OBJ_DIR)Discovery.o $(OBJ_DIR)Ingredient.o $(OBJ_DIR)StatusEffect.o
	$(CC) $(OBJ_DIR)main.o $(OBJ_DIR)Instructor.o $(OBJ_DIR)Alchemist.o \
	$(OBJ_DIR)Discovery.o $(OBJ_DIR)Ingredient.o $(OBJ_DIR)StatusEffect.o \
	-o potions

$(OBJ_DIR)main.o: $(SRC_DIR)main.cpp $(OBJ_DIR)Instructor.o $(OBJ_DIR)Alchemist.o
	$(CC) $(CFLAGS) $(SRC_DIR)main.cpp -o $(OBJ_DIR)main.o

$(OBJ_DIR)Instructor.o: $(SRC_DIR)Instructor.cpp $(SRC_DIR)Instructor.h \
					  $(OBJ_DIR)Alchemist.o
	$(CC) $(CFLAGS) $(SRC_DIR)Instructor.cpp -o $(OBJ_DIR)Instructor.o

$(OBJ_DIR)Alchemist.o: $(SRC_DIR)Alchemist.cpp $(SRC_DIR)Alchemist.h \
					   $(SRC_DIR)WeightedRandomizedSet.h \
					   $(OBJ_DIR)Ingredient.o $(OBJ_DIR)StatusEffect.o
	$(CC) $(CFLAGS) $(SRC_DIR)Alchemist.cpp -o $(OBJ_DIR)Alchemist.o

$(OBJ_DIR)Discovery.o: $(SRC_DIR)Discovery.cpp $(SRC_DIR)Discovery.h \
					   $(OBJ_DIR)Ingredient.o $(OBJ_DIR)StatusEffect.o
	$(CC) $(CFLAGS) $(SRC_DIR)Discovery.cpp -o $(OBJ_DIR)Discovery.o

$(OBJ_DIR)Ingredient.o: $(SRC_DIR)Ingredient.cpp $(SRC_DIR)Ingredient.h  \
						$(OBJ_DIR)StatusEffect.o
	$(CC) $(CFLAGS) $(SRC_DIR)Ingredient.cpp -o $(OBJ_DIR)Ingredient.o

$(OBJ_DIR)StatusEffect.o: $(SRC_DIR)StatusEffect.cpp $(SRC_DIR)StatusEffect.h \
						  $(SRC_DIR)WeightedRandomizedSet.h
	$(CC) $(CFLAGS) $(SRC_DIR)StatusEffect.cpp -o $(OBJ_DIR)StatusEffect.o

clean:
	rm -rf $(OBJ_DIR)*o potions
