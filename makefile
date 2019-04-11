SRCDIR = src
INCDIR = $(SRCDIR)/include
OBJDIR = $(SRCDIR)/obj
$(shell mkdir -p $(SRCDIR)/obj)

_DEPS = file-manager.h
DEPS = $(patsubst %, $(INCDIR)/%, $(_DEPS))

_OBJS = main.o file-manager.o
OBJS = $(patsubst %, $(OBJDIR)/%, $(_OBJS))

GCC = g++
CFLAGS = -I$(INCDIR) -Wall

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	$(GCC) -c -o $@ $< $(CFLAGS)

gftinoc: $(OBJS)
	$(GCC) $^ -o $@ $(CFLAGS)

.PHONY: clean
