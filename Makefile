# Makefile template

INCLDIR = ./include
CC = g++
CFLAGS = -O0 -g -Wall
CFLAGS += -I$(INCLDIR)
LINK = -lm -lSDL2 -lSDL2_image -lSDL2_ttf# libs
CFLAGS += $(LINK)
OUTDIR = ./bin
OUTNAME = znake
OUT = $(OUTDIR)/$(OUTNAME)#executable

OBJDIR = ./obj

_DEPS = window.hpp# header files
_DEPS += game.hpp# header files
_DEPS += input_handler.hpp# header files
_DEPS += renderer.hpp# header files
_DEPS += snake.hpp# header files
_DEPS += deltatime.hpp# header files
_DEPS += tile.hpp# header files
_DEPS += util.hpp# header files
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = main.o# obj files
_OBJS += window.o# obj files
_OBJS += game.o# obj files
_OBJS += input_handler.o# obj files
_OBJS += renderer.o# obj files
_OBJS += snake.o# obj files
_OBJS += deltatime.o# obj files
_OBJS += util.o# obj files
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUT): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~ $(OUTDIR)/$(OUTNAME)
