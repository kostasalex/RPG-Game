EXECUTABLE = rpg

CC = g++
CFLAGS = -c -g -Wall


$(EXECUTABLE): living.o item.o grid.o spell.o main.o
	@echo " Compile rpg ...";
	$(CC) -o $(EXECUTABLE) living.o item.o grid.o spell.o main.o

main.o: main.cpp grid.h
	$(CC) $(CFLAGS) main.cpp

	
living.o: living.cpp living.h inputHandler.h
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
