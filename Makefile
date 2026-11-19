CC = C:/raylib/w64devkit/bin/gcc
FLAGS = -Wall -g
RAYLIB_DIR = C:/raylib/raylib

ASSET_DIR = assets
BUILD_DIR = build

GAME_DIRS := $(patsubst %/,%,$(wildcard src/*/))
GAMES := $(notdir $(GAME_DIRS))
TARGETS := $(addsuffix .exe,$(GAMES))

.PHONY: all assets $(GAMES) $(addsuffix -assets,$(GAMES)) run clean

all: $(TARGETS)

$(GAMES): %: %.exe

# sprite.png -> sprite_png.h (same name xxd uses for the array)
hdr_name = $(subst -,_,$(subst .,_,$(1))).h

# $(1) = game; list of per-asset headers for that game
asset_hdrs = $(foreach f,$(notdir $(wildcard $(ASSET_DIR)/$(1)/*)),$(BUILD_DIR)/$(1)/$(call hdr_name,$(f)))

# $(1) = game, $(2) = asset file name
# cd into the asset folder so xxd names the array "sprite_png", not "assets_game_sprite_png"
define ASSET_template
$(BUILD_DIR)/$(1)/$(call hdr_name,$(2)): $(ASSET_DIR)/$(1)/$(2)
	@mkdir -p $$(@D)
	cd $(ASSET_DIR)/$(1) && wsl xxd -i $(2) > "$(CURDIR)/$$@"
endef

define GAME_template
ifneq ($$(call asset_hdrs,$(1)),)
$(BUILD_DIR)/$(1)_assets.h: $$(call asset_hdrs,$(1))
	cat $$^ > $$@

src/$(1)/$(1)_assets.h: $(BUILD_DIR)/$(1)_assets.h
	cp $$< $$@

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

$(foreach game,$(GAMES),$(foreach f,$(notdir $(wildcard $(ASSET_DIR)/$(game)/*)),$(eval $(call ASSET_template,$(game),$(f)))))
$(foreach game,$(GAMES),$(eval $(call GAME_template,$(game))))

assets: $(ASSET_TARGETS)

GAME ?= snake

run: $(GAME).exe
	./$(GAME).exe

clean:
	$(RM) $(TARGETS)
	rm -rf $(BUILD_DIR) $(foreach g,$(GAMES),src/$(g)/$(g)_assets.h)