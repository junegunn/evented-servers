# Junegunn Choi (junegunn.c@gmail.com)
# 2013/09/03-

BINDIR   = bin
BIN      = $(addprefix $(BINDIR)/, $(notdir $(basename $(wildcard src/*.cc))))

SRC      = $(wildcard lib/*.cc)
HEADERS  = $(wildcard lib/*.h)
CFLAGS   = -O2 -Wall -std=c++11 -stdlib=libc++
INCLUDE  = lib

#-------------------------------------------------
# Compiler options -------------------------------

CC       = clang++
CFLAGS  += $(addprefix -I, $(INCLUDE)) -fPIC
LFLAGS   = -lev
OBJ      = $(addprefix $(OBJDIR)/, $(notdir $(SRC:.cc=.o)))
OBJDIR   = obj

# Build ------------------------------------------

all: $(BIN)

# targets: normal-prerequisites | order-only-prerequisites
$(OBJ): | $(OBJDIR)
$(OBJDIR):
	mkdir -p $(OBJDIR)
$(OBJDIR)/%.o: lib/%.cc $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): | $(BINDIR)
$(BINDIR):
	mkdir -p $(BINDIR)
$(BINDIR)/%: src/%.cc $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)

# Clean ------------------------------------------

clean:
	-rm -rf $(OBJDIR) $(BINDIR) core

.PHONY: clean all

#-------------------------------------------------

