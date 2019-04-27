SRCDIR = src
INCDIR = $(SRCDIR)/include
OBJDIR = $(SRCDIR)/obj
$(shell mkdir -p $(SRCDIR)/obj)

_DEPS = graph-representation.h rng.h adjacency-list.h adjacency-matrix.h \
		tabu-adj-matrix.h file-manager.h graph-converter.h #dijkstra.h tabu-search.h
DEPS = $(patsubst %, $(INCDIR)/%, $(_DEPS))

_OBJS = main.o file-manager.o
OBJS = $(patsubst %, $(OBJDIR)/%, $(_OBJS))

_INLS = graph-representation.inl adjacency-list.inl adjacency-matrix.inl \
		tabu-adj-matrix.inl graph-converter.inl #dijkstra.inl tabu-search.inl
INLS = $(patsubst %, $(SRCDIR)/%, $(_INLS))

GCC = g++
CFLAGS = -std=c++11 -I$(INCDIR) -Wall
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
