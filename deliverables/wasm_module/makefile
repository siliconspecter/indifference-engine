CC = clang
COMMON_CFLAGS = -Wall -Wextra -Werror -std=c99 -target wasm32 -nostdlib -ffreestanding
PRODUCTION_CFLAGS = $(COMMON_CFLAGS) -flto -O3
DEVELOPMENT_CFLAGS = $(COMMON_CFLAGS) -O0

C_FILES = $(shell find source -type f -iname "*.c")
H_FILES = $(shell find source -type f -iname "*.h")
PRODUCTION_O_FILES = $(patsubst source/%.c,ephemeral/production/objects/%.o,$(C_FILES))
DEVELOPMENT_O_FILES = $(patsubst ephemeral/production/%,ephemeral/development/%,$(PRODUCTION_O_FILES))
TOTAL_REBUILD_FILES = makefile $(H_FILES)

production: ephemeral/production/build/module.wasm
development: ephemeral/development/build/module.wasm
documentation:
	rm -rf ephemeral/documentation
	doxygen

clean:
	find ephemeral -mindepth 1 ! -name '.gitignore' -exec rm -rf {} +

ephemeral/production/objects/%.o: source/%.c $(TOTAL_REBUILD_FILES)
	mkdir -p $(dir $@)
	$(CC) $(PRODUCTION_CFLAGS) -c $< -o $@

ephemeral/development/objects/%.o: source/%.c $(TOTAL_REBUILD_FILES)
	mkdir -p $(dir $@)
	$(CC) $(DEVELOPMENT_CFLAGS) -c $< -o $@

ephemeral/production/build/module.wasm: $(PRODUCTION_O_FILES)
	mkdir -p $(dir $@)
	$(CC) $(PRODUCTION_CFLAGS) -Wl,--no-entry -Wl,--export-dynamic $(PRODUCTION_O_FILES) -o $@

ephemeral/development/build/module.wasm: $(DEVELOPMENT_O_FILES)
	mkdir -p $(dir $@)
	$(CC) $(DEVELOPMENT_CFLAGS) -Wl,--no-entry -Wl,--export-dynamic $(PRODUCTION_O_FILES) -o $@
