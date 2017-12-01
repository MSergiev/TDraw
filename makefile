CC = g++
CFLAGS = -I. -Wall
DEPS = *.cpp
OBJ = *.o 
LIBS = -lncursesw -lGLEW -lGL 

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

draw: $(OBJ)
	$(CC) -o $@ $^ $(CFLAG) $(LIBS) $(ncursesw5-config --cflags)