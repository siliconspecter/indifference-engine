CC = clang
COMMON_CFLAGS = -Wall -Wextra -Werror -std=c99 -target wasm32 -nostdlib -ffreestanding
PRODUCTION_CFLAGS = $(COMMON_CFLAGS) -flto -O3
DEVELOPMENT_CFLAGS = $(COMMON_CFLAGS) -O0

ifeq ($(OS),Windows_NT)
	TOOL_CC = x86_64-w64-mingw32-gcc
else
	TOOL_CC = $(CC)
endif
TOOL_CFLAGS = -Wall -Wextra -Werror -std=c99 -O3

C_FILES = $(shell find source -type f -iname "*.c" -not -path "source/tools/*")
H_FILES = $(shell find source -type f -iname "*.h")
TGA_FILES = $(shell find source -type f -iname "*.tga")
OBJ_FILES = $(shell find source -type f -iname "*.obj")
TGA2C_H_FILES = source/engine/primitives/f32.h source/engine/assets/texture.h
OBJ2C_H_FILES = source/engine/primitives/f32.h source/engine/primitives/index.h source/engine/primitives/quantity.h source/engine/assets/mesh.h source/engine/assets/navigation_mesh.h source/engine/math/vector.h
PRODUCTION_O_FILES = $(patsubst source/%.c,ephemeral/production/objects/source/%.o,$(C_FILES)) $(patsubst source/%.tga,ephemeral/production/objects/ephemeral/tga/%.o,$(TGA_FILES)) $(patsubst source/%.obj,ephemeral/production/objects/ephemeral/obj/%.o,$(OBJ_FILES))
DEVELOPMENT_O_FILES = $(patsubst ephemeral/production/%,ephemeral/development/%,$(PRODUCTION_O_FILES))
TOTAL_REBUILD_FILES = makefile $(H_FILES)

default:
	$(error Please run "make production", or "make development" or "make clean" and add " --jobs" for faster builds with less clear error messages)

production: ephemeral/production/build/module.wasm
development: ephemeral/development/build/module.wasm

clean:
	find ephemeral -mindepth 1 ! -name '.gitignore' -exec rm -rf {} +

ephemeral/tools/%: source/tools/%.c makefile
	mkdir -p $(dir $@)
	$(TOOL_CC) $(TOOL_CFLAGS) $< -o $@

ephemeral/tga/%.c: source/%.tga ephemeral/tools/tga2c
	mkdir -p $(dir $@)
	ephemeral/tools/tga2c $(realpath $(TGA2C_H_FILES)) $(subst /,_,$(patsubst source/%.tga,%,$<)) < $< > $@.temp
	mv $@.temp $@

# TODO: 2 and 4 are the same
ephemeral/obj/%.c: source/%.obj ephemeral/tools/obj2c
	mkdir -p $(dir $@)
	ephemeral/tools/obj2c $(realpath $(OBJ2C_H_FILES)) $(subst /,_,$(patsubst source/%/,%,$(dir $<))) $(subst /,_,$(patsubst %.obj,%,$(notdir $<))) $(dir $<) $(patsubst source_%_,%,$(subst /,_,$(dir $<)))_ < $< > $@.temp
	mv $@.temp $@

ephemeral/production/objects/%.o: %.c $(TOTAL_REBUILD_FILES)
	mkdir -p $(dir $@)
	$(CC) $(PRODUCTION_CFLAGS) -c $< -o $@

ephemeral/development/objects/%.o: %.c $(TOTAL_REBUILD_FILES)
	mkdir -p $(dir $@)
	$(CC) $(DEVELOPMENT_CFLAGS) -c $< -o $@

ephemeral/production/build/module.wasm: $(PRODUCTION_O_FILES)
	mkdir -p $(dir $@)
	$(CC) $(PRODUCTION_CFLAGS) -Wl,--no-entry -Wl,--export-dynamic $(PRODUCTION_O_FILES) -o $@

ephemeral/development/build/module.wasm: $(DEVELOPMENT_O_FILES)
	mkdir -p $(dir $@)
	$(CC) $(DEVELOPMENT_CFLAGS) -Wl,--no-entry -Wl,--export-dynamic $(DEVELOPMENT_O_FILES) -o $@
