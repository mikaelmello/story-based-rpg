CPP      = g++
CC       = gcc
SRCS  	 = $(shell find src -name '*.cpp')
OBJ      = $(addprefix obj/,$(notdir $(SRCS:%.cpp=%.o))) 
LIBS     = 
INCS     = 
CXXINCS  = -I"include/" -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer 
BIN      = engine
CXXFLAGS = $(CXXINCS) -std=c++14 -O3
CFLAGS   = $(INCS) -std=c14 O3
RM       = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

debug: CXXFLAGS = $(CXXINCS) -std=c++14 -g3 -ggdb3
debug: CCFLAGS  = $(INCS) -std=c14 -g3 -ggdb3
debug: all

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(OBJ) -o $(BIN) $(LIBS) $(CXXFLAGS)

obj/%.o: src/%.cpp
	$(CPP) -c $< -o $@ $(CXXFLAGS)