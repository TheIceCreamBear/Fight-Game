# Compiler variables
CC=gcc
FLAGS=-Wall
INC=-Isrc -Iinclude
LIB_FLAGS=-lm
DIR_GUARD=@mkdir -p $(@D)

# Build configs
CFG=release
ifeq ($(CFG), debug)
	FLAGS += -g3 -O0 -DDEBUG
endif
ifneq ($(CFG),debug)
ifneq ($(CFG),release)
	@echo "Invalid configuration " $(CFG) "."
	@echo "Choice are 'release', 'debug'."
	@exit 1
endif
endif

# Sources
SOURCES=src/*.c

# Main targets
.PHONY: all clean

all: bin/$(CFG)/fight

clean:
	rm -rf bin/$(CFG)/*

# Compile
bin/$(CFG)/fight:
	$(DIR_GUARD)
	$(CC) $(FLAGS) $(INC) $(SOURCES) -o bin/$(CFG)/fight $(LIB_FLAGS)

