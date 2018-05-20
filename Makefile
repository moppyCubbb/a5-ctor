CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD
OBJECTS = Player.o Address.o Path.o Tile.o Board.o Game.o main.o TextDisplay.o Random.o Geese.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = ctor
${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}
-include ${DEPENDS}
.PHONY : clean
clean:
	rm ${DEPENDS} ${OBJECTS} ${EXEC}
