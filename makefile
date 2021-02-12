EXECUTABLE = rpg

CC = g++
CFLAGS = -c -g -Wall


$(EXECUTABLE): game.o living.o item.o grid.o
	@echo " Compile rpg ...";
	$(CC) -o $(EXECUTABLE) game.o living.o item.o grid.o
	
game.o: game.cpp living.h item.h grid.h
	$(CC) $(CFLAGS) game.cpp
	
living.o: living.cpp living.h
	$(CC) $(CFLAGS) living.cpp	

grid.o: grid.cpp grid.h
	$(CC) $(CFLAGS) grid.cpp

item.o: item.cpp item.h
	$(CC) $(CFLAGS) item.cpp

.phony: clean

clean:
	@echo " Cleaning . . ."
	rm -f *.o $(EXECUTABLE)
