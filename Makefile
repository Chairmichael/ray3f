CC = C:/raylib/w64devkit/bin/gcc
FLAGS = -Wall -g
RAYLIB_DIR = C:/raylib/raylib
TARGET = game.exe
SOURCES = src/main.c src/game.c

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(FLAGS) $(SOURCES) -o $@ \
		-I$(RAYLIB_DIR)/src \
		-I$(RAYLIB_DIR)/examples/core \
		-L$(RAYLIB_DIR)/src \
		-lraylib -lopengl32 -lgdi32 -lwinmm

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(TARGET)