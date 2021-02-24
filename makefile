EXECUTABLE = rpg

BUILD = build
SRC = src
INC = include


<<<<<<< HEAD
SOURCE = $(wildcard $(SRC)/*.cpp)
	
OBJECT = $(patsubst %,$(BUILD)/%, $(notdir $(SOURCE:.cpp=.o)))

CC = g++
CFLAGS = -Wall -g -I$(INC)

GREEN = \033[1;32m
BLUE = \033[1;34m
YELLOW = \033[1;33m
NC = \033[1;0m
	
	
$(BUILD)/$(EXECUTABLE) : $(OBJECT)
	@echo "$ Compiling...$(NC)"
	$(CC) -o $@ $^
	
$(BUILD)/%.o : $(SRC)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
=======
$(EXECUTABLE): living.o item.o grid.o spell.o main.o
	@echo " Compile rpg ...";
	$(CC) -o $(EXECUTABLE) living.o item.o grid.o spell.o main.o

main.o: main.cpp grid.h
	$(CC) $(CFLAGS) main.cpp

	
living.o: living.cpp living.h inputHandler.h
	$(CC) $(CFLAGS) living.cpp	
>>>>>>> main



.PHONY: run valgrind clean


run: $(BUILD)/$(EXECUTABLE)
	@echo "$(GREEN) Executing..$(NC)"
	$(BUILD)/$(EXECUTABLE)
	
	
valgrind: $(BUILD)/$(EXECUTABLE)
	@echo "$(YELLOW)👓️ Debugging..$(NC)"
	valgrind ./$(BUILD)/$(EXECUTABLE)	
	
	
clean: 
	@echo "$(BLUE)🧹 Cleaning..$(NC)"
	rm -f $(OBJECT) $(BUILD)/$(EXECUTABLE)
	
