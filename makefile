EXECUTABLE = rpg

CC = g++
CFLAGS = -c -g -Wall


$(EXECUTABLE): game.o living.o
	@echo " Compile rpg ...";
	$(CC) -o $(EXECUTABLE) game.o living.o
	
game.o: game.cpp living.h
	$(CC) $(CFLAGS) game.cpp
	
living.o: living.cpp living.h
	$(CC) $(CFLAGS) living.cpp	

.phony: clean

clean:
	@echo " Cleaning . . ."
	rm -f *.o $(EXECUTABLE)
