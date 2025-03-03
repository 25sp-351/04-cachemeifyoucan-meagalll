CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: rodcut_lru rodcut_fifo

rodcut_lru: rodcut.c cache_lru.c piece_values.c cut_list.c vec.c
	$(CC) $(CFLAGS) -D CACHE_POLICY_LRU -o $@ $^

rodcut_fifo: rodcut.c cache_fifo.c piece_values.c cut_list.c vec.c
	$(CC) $(CFLAGS) -D CACHE_POLICY_FIFO -o $@ $^

clean:
	rm -f rodcut_lru rodcut_fifo

# Ignore compiled binaries
*.o
*.out
*.exe
my_program  # Name of your compiled binary

# Ignore debug files
*.dSYM/
*.stackdump

# Ignore VSCode settings
.vscode/
*.code-workspace

# Ignore OS-specific files
.DS_Store
Thumbs.db

# Ignore build artifacts
build/
Makefile~
