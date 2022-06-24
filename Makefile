# Makefile template

INCLDIR = ./include
CC = g++
CFLAGS = -O0 -Wall
CFLAGS += -I$(INCLDIR)
LINK = -lm -lSDL2 -lSDL2_image -lSDL2_ttf# libs
CFLAGS += $(LINK)
OUTDIR = ./bin
OUTNAME = snake
OUT = $(OUTDIR)/$(OUTNAME)#executable

OBJDIR = ./obj

_DEPS = window.h# header files
_DEPS += game.h# header files
_DEPS += input_handler.h# header files
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = main.o# obj files
_OBJS += window.o# obj files
_OBJS += game.o# obj files
_OBJS += input_handler.o# obj files
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUT): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~ $(OUTDIR)/$(OUTNAME)
