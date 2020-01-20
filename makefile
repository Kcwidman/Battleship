prog: main.o ship.o board.o
	g++ -std=c++11 -g -Wall main.o ship.o board.o -o prog

main.o: main.cpp ship.h board.h
	g++ -std=c++11 -g -Wall -c main.cpp

ship.o: ship.h ship.cpp
	g++ -std=c++11 -g -Wall -c ship.cpp 

board.o: board.h board.cpp
	g++ -std=c++11 -g -Wall -c board.cpp 

clean: 
	rm *.o main.cpp