# ----- File Definitions -----
OBJS += src/main.o
BIN ?= build/bin

BLIB ?= ../..
BLIB_INCLUDE ?= $(BLIB)/include
BLIB_LIB ?= $(BLIB)/lib
LIBUTIL ?= ..
LIBUTIL_INCLUDE ?= $(LIBUTIL)/include
LIBUTIL_LIB ?= $(LIBUTIL)/lib

# ----- Program Definitions -----
CC ?= gcc
CCLD ?= $(CC)

RM ?= rm

# ----- Program Flags -----
WFLAGS += -Wall -Wextra -Wpedantic -Werror
CFLAGS += $(WFLAGS) -O2
IFLAGS += -I$(BLIB_INCLUDE) -I$(LIBUTIL)
LDFLAGS += -L$(BLIB_LIB) -lb -L$(LIBUTIL_LIB) -lutil

RM_FLAGS ?= -f
CLEAN ?= $(RM) $(RM_FLAGS)

# ----- Highlevel Targets -----
all : $(LIBUTIL) $(BIN)

# ----- Build Objects -----
.SUFFIXES: .c .o

.c.o:
	@echo "  CC    $@"
	@ $(CC) -o $@ $< $(CFLAGS) -c $(IFLAGS)

# ----- Lower level Targets -----
$(BIN): $(OBJS)
	@echo "  CCLD  $@"
	@ $(CCLD) -o $@ $(OBJS) $(LDFLAGS)

$(LIBUTIL):
	@echo "  MAKE  $@"
	@ $(MAKE) -C $(LIBUTIL) all

# ----- Convenience Targets -----
clean:
	@echo "  CLEAN $(OBJS) $(BIN)"
	@ $(CLEAN) $(OBJS) $(BIN)
	@echo "  CLEAN $(LIBUTIL)"
	@ $(MAKE) -C $(LIBUTIL) clean
