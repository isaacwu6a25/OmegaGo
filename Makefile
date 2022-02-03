
CXX=g++
CPPFLAGS=-std=c++17 -Wall

twoplayer: twoplayer.o engine2.o
	${CXX} ${CPPFLAGS} twoplayer.o engine2.o glad.c -o twoplayer -lglfw -framework OpenGl

twoplayer.o: twoplayer.cpp line.h board_gui.h
	${CXX} ${CPPFLAGS} twoplayer.cpp -c

# twoplayer: twoplayer.cpp line.h
# 	${CXX} ${CPPFLAGS} twoplayer.cpp glad.c -o twoplayer -lglfw -framework OpenGl

# engine2.o: engine2.cpp engine2.h
# 	${CC} ${CFLAGS} engine2.cpp -c
