# ----- File Definitions -----
OBJS += src/test.o src/datastructures/arrays/dynamic.o
BIN  ?= build/test

LIB_DIR ?= lib

LINCLUDE_DIR ?= $(LIB_DIR)/include
LLIB_DIR ?= $(LIB_DIR)/lib

BLIB_DIR ?= ..

FLAGS ?= compile_flags.txt

# ----- Program Definitions -----
CC ?= gcc
CCLD ?= $(CC)
RM ?= rm

CLEAN ?= $(RM) $(RM_FLAGS)

# ----- Program Flags -----
WFLAGS += -Wall -Wextra -Wpedantic -Werror
CFLAGS += $(WFLAGS) -O2 -I$(BLIB_DIR)/include -I$(LINCLUDE_DIR)
LDFLAGS += -L$(BLIB_DIR)/lib -lb -L$(LLIB_DIR) -lutil

RM_FLAGS ?= -rf

EXTCFLAGS ?= ""

# ----- High Level Targets -----
all : $(BIN)

debug:
	@ CC="$(CC)" CFLAGS="-ggdb" EXTCFLAGS="-ggdb" CCLD="$(CCLD)" LDFLAGS="-ggdb" RM="$(RM)" RM_FLAGS="$(RM_FLAGS)" CLEAN="$(CLEAN)" \
		LINCLUDE_DIR="$(LINCLUDE_DIR)" LLIB_DIR="$(LLIB_DIR)" BLIB_DIR="$(BLIB_DIR)" FLAGS="$(FLAGS)" $(MAKE) all

# ----- Build Objects -----
.SUFFIXES: .c .o
.c.o:
	@echo "  CC    $@"
	@ $(CC) -o $@ $< $(CFLAGS) -c

# ----- Lower Level Targets -----
$(BIN): $(LIB_DIR) $(OBJS)
	@echo "  CCLD  $@"
	@ $(CCLD) -o $@ $(OBJS) $(LDFLAGS)

$(LIB_DIR):
	@echo "  MAKE  $@"
	@ $(MAKE) -C $(LIB_DIR) all \
		CC="$(CC)" CCLD="$(CCLD)" RM="$(RM)" RM_FLAGS="$(RM_FLAGS)" CFLAGS="$(EXTCFLAGS)"

# ----- Convenience Targets -----
clean:
	@echo "  CLEAN $(OBJS) $(BIN)"
	@ $(CLEAN) $(OBJS) $(BIN)
	@echo "  CLEAN $(LIB_DIR)"
	@ $(MAKE) -C $(LIB_DIR) clean

flags:
	@echo $(CFLAGS) | sed 's/ /\n/g' > $(FLAGS)

.PHONY: $(LIB_DIR) clean flags
