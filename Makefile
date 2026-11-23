CC = C:/raylib/w64devkit/bin/gcc
FLAGS = -Wall -g
RAYLIB_DIR = C:/raylib/raylib

PYTHON ?= python
ASSET_DIR = assets
ASSET_TOOL = tools/gen_assets.py

GAME_DIRS := $(patsubst %/,%,$(wildcard src/*/))
GAMES := $(notdir $(GAME_DIRS))
TARGETS := $(addsuffix .exe,$(GAMES))

.PHONY: all assets $(GAMES) $(addsuffix -assets,$(GAMES)) run clean

all: $(TARGETS)

$(GAMES): %: %.exe

define GAME_template
ifneq ($$(wildcard $(ASSET_DIR)/$(1)/*),)
src/$(1)/$(1)_assets.h: $$(wildcard $(ASSET_DIR)/$(1)/*) $(ASSET_TOOL)
	$$(PYTHON) $(ASSET_TOOL) $(ASSET_DIR)/$(1) $$@

GAME_ASSET_HDR_$(1) := src/$(1)/$(1)_assets.h
ASSET_TARGETS += src/$(1)/$(1)_assets.h
endif

$(1)-assets: $$(GAME_ASSET_HDR_$(1))

$(1).exe: $$(wildcard src/$(1)/*.c) $$(wildcard src/$(1)/*.h) $$(GAME_ASSET_HDR_$(1))
	$$(CC) $$(FLAGS) $$(wildcard src/$(1)/*.c) -o $$@ \
		-I$$(RAYLIB_DIR)/src \
		-I$$(RAYLIB_DIR)/examples/core \
		-L$$(RAYLIB_DIR)/src \
		-lraylib -lopengl32 -lgdi32 -lwinmm
endef

$(foreach game,$(GAMES),$(eval $(call GAME_template,$(game))))

assets: $(ASSET_TARGETS)

GAME ?= snake

run: $(GAME).exe
	./$(GAME).exe

clean:
	$(RM) $(TARGETS)
	$(RM) $(foreach g,$(GAMES),src/$(g)/$(g)_assets.h)