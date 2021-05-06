
CC=gcc
FLAGS=-Wall -Wpedantic
INC=-Iinclude/
LIB_FLAGS=-lm # -lpthread in the future?
DIR_GUARD=@mkdir -p $(@D)
DEBUG_FLAGS=-g -DDEBUG -DWNO_ERROR -O0

# Build configurations
CFG=release
ifeq ($(CFG), debug)
	FLAGS += $(DEBUG_FLAGS)
endif
ifneq ($(CFG),debug)
ifneq ($(CFG),release)
	@echo "Invalid configuration " $(CFG) "."
	@echo "Choices are 'release', 'debug'."
	@exit 1
endif
endif

# Sources
SOURCES=src/logging.c src/main.c src/object.c src/room.c src/roomtype.c src/utils.c src/fight.c

# Main targets
.PHONY: all clean

all: bin/$(CFG)/fight

clean: rm -rf bin/$(CFG)/*

clean_all:
	rm -rf bin/*

# Compile
bin/$(CFG)/fight:
	$(DIR_GUARD)
	$(CC) $(INC) $(FLAGS) $(SOURCES) -o bin/$(CFG)/room $(LIB_FLAGS)
