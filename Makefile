all: compile link

compile: 
	g++ -I src/include -c main.cpp board.cpp texturemanager.cpp

link: 
	g++ main.o board.o texturemanager.o -o main -L src/lib -l sfml-graphics -l sfml-window -l sfml-system