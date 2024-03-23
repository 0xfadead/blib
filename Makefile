# ----- File definitions -----
OBJS += src/datastructures/arrays/dynamic.o src/testing/time/time_tests.o
OUT ?= $(BUILD_DIR)/libb.$(LIB_EXT)

INCLUDE_DIR ?= include

LIB_EXT ?= a
BUILD_DIR ?= lib

FLAGS_FILE ?= compile_flags.txt

TESTS_DIR ?= tests

# ----- Program definitions -----
CC ?= gcc

AR ?= ar

RM ?= rm
CLEAN ?= $(RM) $(RMFLAGS)

# ----- Program flags ------
WFLAGS += -Wall -Wextra -Wpedantic -Werror
IFLAGS += -I$(INCLUDE_DIR)
CFLAGS += $(WFLAGS) $(IFLAGS) -O2 -std=c99

DBGFLAGS ?= -ggdb -DDEBUG=1

ARFLAGS ?= rcs

RMFLAGS ?= -f --

TEST_NUM ?= 1024

# ----- High level targets -----
all : $(OUT)

debug:
	@ $(MAKE) CFLAGS="$(CFLAGS) $(DBGFLAGS)"

tests: all
	@echo "  MAKE  $(TESTS_DIR)"
	@ $(MAKE) -C $(TESTS_DIR) all

debug_tests: clean debug
	@echo "  MAKE  $(TESTS_DIR)"
	@ $(MAKE) -C $(TESTS_DIR) debug

run_tests:
	@ $(TESTS_DIR)/build/test $(TEST_NUM)

# ----- Build object files -----
.SUFFIXES: .c .o

src/datastructures/arrays/dynamic.o: include/blib/datastructures/arrays/dynamic.h
src/testing/time/time_tests.o: include/blib/testing/time/time_tests.h
.c.o:
	@echo "  CC    $@"
	@ $(CC) -o $@ $< $(CFLAGS) -c

# ----- Lower level targets -----
$(OUT): $(OBJS)
	@echo "  AR    $@"
	@ $(AR) $(ARFLAGS) $@ $(OBJS)

# ----- Convenience targets -----
.PHONY: clean flags

clean:
	@echo "  CLEAN $(OBJS) $(OUT)"
	@ $(CLEAN) $(OBJS) $(OUT)

realclean: clean
	@echo "  MAKE  $(TESTS_DIR)"
	@ $(MAKE) -C $(TESTS_DIR) clean

flags:
	@ echo $(CFLAGS) | sed 's/ /\n/g' > $(FLAGS_FILE)
