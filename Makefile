# Junegunn Choi (junegunn.c@gmail.com)
# 2013/09/03-

VERSION  = 0.0.1

BINDIR   = bin
BIN      = $(addprefix $(BINDIR)/, $(notdir $(basename $(wildcard src/*.cc))))

SRC      = $(wildcard lib/*.cc)
HEADERS  = $(wildcard lib/*.h)
INCLUDE  = lib

#-------------------------------------------------
# Compiler options -------------------------------

CC       = g++
CFLAGS   = -O2 -Wall -DVERSION='"$(VERSION)"' -std=c++11
ifeq ($(MAKECMDGOALS), clang)
CC       = clang++
CFLAGS  += -stdlib=libc++
endif
CFLAGS  += $(addprefix -I, $(INCLUDE)) -fPIC

LFLAGS   = -lev
ifeq ($(static), 1)
LFLAGS  += -static
endif

OBJ      = $(addprefix $(OBJDIR)/, $(notdir $(SRC:.cc=.o)))
OBJDIR   = obj

# Build ------------------------------------------

error:
	@echo "make gcc|clang|clean [static=1]"
	@exit 1

clang: $(BIN)
gcc: $(BIN)

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

