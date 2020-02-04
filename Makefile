CXX = g++
CXXFLAGS = --std=c++14 -Wall -MMD -g
OBJECTS = request.o horsin.o main.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = horsin-around

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lcurl -ljsoncpp

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${EXEC} ${OBJECTS} ${DEPENDS}

