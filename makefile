all: client server

server: server.o
	g++ -std=c++14 -o server.out server.cpp

client: client.o
	g++ -std=c++14 -o client.out client.cpp

server.o: server.cpp
	g++ -std=c++14 -c server.cpp

client.o: client.cpp
	g++ -std=c++14 -c client.cpp

clean:
	rm *.o *.out