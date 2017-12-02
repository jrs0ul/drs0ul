CC=g++
LDFLAGS= -lGL -lSDL -lGLU -logg -lvorbis -lvorbisfile -lopenal -lpthread
SOURCES= src/*.cpp
EXECUTABLE=drs0ul

all : 
	$(CC)  $(SOURCES) $(LDFLAGS) -o $(EXECUTABLE)
	

