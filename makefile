SRCDIR = src
INCDIR = $(SRCDIR)/include
OBJDIR = $(SRCDIR)/obj
$(shell mkdir -p $(SRCDIR)/obj)

_DEPS = file-manager.h graph-representation.h adjacency-list.h \
		adjacency-matrix.h graph-converter.h
DEPS = $(patsubst %, $(INCDIR)/%, $(_DEPS))

_OBJS = main.o file-manager.o
OBJS = $(patsubst %, $(OBJDIR)/%, $(_OBJS))

_INLS = adjacency-matrix.inl graph-representation.inl adjacency-list.inl \
		graph-converter.inl
INLS = $(patsubst %, $(SRCDIR)/%, $(_INLS))

GCC = g++
CFLAGS = -I$(INCDIR) -Wall
DEBUGFLAGS = -g -O0

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS) $(INLS)
	$(GCC) -c -o $@ $< $(CFLAGS) $(DEBUGFLAGS)

gftinoc: $(OBJS)
	$(GCC) $^ -o $@ $(CFLAGS)

debug: $(OBJS)
	$(GCC)  $^ -o $@ $(CFLAGS) 

clean:
	rm -f $(OBJDIR)/*.o

.PHONY: clean
