CXX     := g++
CC      := gcc

LDFLAGS :=
CFLAGS 	:= -Wall -g -std=c11
CXXFLAGS := -Wall -g -std=c++11

# build folder
BUILD = build

C_SOURCES   := ableHandler.c
CPP_SOURCES := linuxTest.c

OBJECTS := $(C_SOURCES:%.c=$(BUILD)/%.o) $(CPP_SOURCES:%.c=$(BUILD)/%.o)

default: $(OBJECTS)
	mkdir -p $(BUILD)
	$(CXX) -g $(OBJECTS) -o run.out $(LDFLAGS)

# Compile C and C++ sources
$(BUILD)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


