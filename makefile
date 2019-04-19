SRCDIR = src
INCDIR = $(SRCDIR)/include
OBJDIR = $(SRCDIR)/obj
$(shell mkdir -p $(SRCDIR)/obj)

_DEPS = file-manager.h graph-representation.h adjacency-list.h \
		adjacency-matrix.h
DEPS = $(patsubst %, $(INCDIR)/%, $(_DEPS))

_OBJS = main.o file-manager.o graph-representation.o adjacency-list.o
OBJS = $(patsubst %, $(OBJDIR)/%, $(_OBJS))

_INLS = adjacency-matrix.inl
INLS = $(patsubst %, $(SRCDIR)/%, $(_INLS))

GCC = g++
CFLAGS = -I$(INCDIR) -Wall

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS) $(INLS)
	$(GCC) -c -o $@ $< $(CFLAGS)

gftinoc: $(OBJS)
	$(GCC) $^ -o $@ $(CFLAGS)

.PHONY: clean
