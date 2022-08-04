CC = g++
INCLDIR = ./include
CFLAGS = -O0 -g -Wall -I$(INCLDIR)
LINK = -lm -lSDL2 -lSDL2_ttf
CFLAGS += $(LINK)
OUTDIR = ./bin
OUTNAME = znake
OUT = $(OUTDIR)/$(OUTNAME)

OBJDIR = ./obj

OBJS =\
main.o window.o game.o input_handler.o \
renderer.o snake.o deltatime.o util.o

$(OUT) : $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)

main.o : ./include/game.hpp
window.o : ./include/window.hpp
game.o : ./include/util.hpp ./include/game.hpp
input_handler.o : ./include/input_handler.hpp
renderer.o : ./include/renderer.hpp ./include/util.hpp
snake.o : ./include/snake.hpp
deltatime.o : ./include/deltatime.hpp
util.o : ./include/util.hpp

.PHONY : clean
clean :
	rm -rf $(OUT) $(OBJS)
