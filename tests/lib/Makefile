
# ----- File Definitions -----
LIB_ENDING ?= a

OBJS += src/util.o
BIN ?= lib/libutil.$(LIB_ENDING)

INCLUDE_DIR ?= include

FLAGS ?= compile_flags.txt

# ----- Program Definitions -----
CC ?= gcc

AR ?= ar
RM ?= rm

CLEAN ?= $(RM) $(RM_FLAGS)

# ----- Program Flags -----
WFLAGS += -Wall -Wextra -Wpedantic -Werror
IFLAGS += -I$(INCLUDE_DIR)
CFLAGS += $(WFLAGS) -O2

AR_FLAGS ?= rcs
RM_FLAGS ?= -rf

# ----- High Level Targets -----
all : $(BIN)

# ----- Build Objects -----
.SUFFIXES: .c .o
.c.o:
	@echo "  CC    $@"
	@ $(CC) -o $@ $< $(CFLAGS) $(IFLAGS) -c

# ----- Lower Level Targets -----
$(BIN): $(OBJS)
	@echo "  AR    $@"
	@ $(AR) $(AR_FLAGS) $(BIN) $(OBJS)

# ----- Convenience Targets -----
clean:
	@echo "  CLEAN $(OBJS) $(BIN)"
	@ $(CLEAN) $(OBJS) $(BIN)

flags:
	@ echo $(CFLAGS) $(IFLAGS) | sed 's/ /\n/g' > $(FLAGS)
