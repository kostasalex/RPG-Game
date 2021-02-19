EXECUTABLE = rpg

CC = g++
CFLAGS = -c -g -Wall


$(EXECUTABLE): game.o living.o item.o grid.o spell.o main.o
	@echo " Compile rpg ...";
	$(CC) -o $(EXECUTABLE) game.o living.o item.o grid.o spell.o main.o

main.o: main.cpp game.h
	$(CC) $(CFLAGS) main.cpp
	
game.o: game.cpp game.h utils.h
	$(CC) $(CFLAGS) game.cpp
	
living.o: living.cpp living.h utils.h
	$(CC) $(CFLAGS) living.cpp	

grid.o: grid.cpp grid.h
	$(CC) $(CFLAGS) grid.cpp

item.o: item.cpp item.h
	$(CC) $(CFLAGS) item.cpp

spell.o: spell.cpp spell.h
	$(CC) $(CFLAGS) spell.cpp

.phony: clean

clean:
	@echo " Cleaning . . ."
	rm -f *.o $(EXECUTABLE)
