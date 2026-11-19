CC = C:/raylib/w64devkit/bin/gcc
FLAGS = -Wall -g
RAYLIB_DIR = C:/raylib/raylib

GAME_DIRS := $(patsubst %/,%,$(wildcard src/*/))
GAMES := $(notdir $(GAME_DIRS))
TARGETS := $(addsuffix .exe,$(GAMES))

.PHONY: all $(GAMES) run clean

all: $(TARGETS)

$(GAMES): %: %.exe

define GAME_template
$(1).exe: $$(wildcard src/$(1)/*.c) $$(wildcard src/$(1)/*.h)
	$$(CC) $$(FLAGS) $$(wildcard src/$(1)/*.c) -o $$@ \
		-I$$(RAYLIB_DIR)/src \
		-I$$(RAYLIB_DIR)/examples/core \
		-L$$(RAYLIB_DIR)/src \
		-lraylib -lopengl32 -lgdi32 -lwinmm
endef

$(foreach game,$(GAMES),$(eval $(call GAME_template,$(game))))

GAME ?= snake

run: $(GAME).exe
	./$(GAME).exe

clean:
	$(RM) $(TARGETS)
