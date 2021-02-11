EXECUTABLE = rpg

CC = g++
CFLAGS = -c -g -Wall


$(EXECUTABLE): game.o living.o item.o
	@echo " Compile rpg ...";
	$(CC) -o $(EXECUTABLE) game.o living.o item.o
	
game.o: game.cpp living.h item.h
	$(CC) $(CFLAGS) game.cpp
	
living.o: living.cpp living.h
	$(CC) $(CFLAGS) living.cpp	

item.o: item.cpp item.h
	$(CC) $(CFLAGS) item.cpp

.phony: clean

clean:
	@echo " Cleaning . . ."
	rm -f *.o $(EXECUTABLE)
