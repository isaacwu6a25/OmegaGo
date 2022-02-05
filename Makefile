CXX=g++
CPPFLAGS=-std=c++17 -Wall

twoplayer: twoplayer.o engine2.o
	${CXX} ${CPPFLAGS} twoplayer.o engine2.o glad.c -o twoplayer -lglfw -framework OpenGl

twoplayer.o: twoplayer.cpp board_gui.h
	${CXX} ${CPPFLAGS} twoplayer.cpp -c

engine2.o: engine2.cpp engine2.h
	${CXX} ${CPPFLAGS} engine2.cpp -c