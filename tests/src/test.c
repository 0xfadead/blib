#include <stdio.h>
#include "datastructures/arrays/dynamic.h"
#include <stdlib.h>
#include <string.h>
#include <util.h>
#include <ctype.h>
#include <limits.h>

#define progname "Testrunner"
#define reset_progname set_prog_name(progname)

void print_inval_strtol(const char* str, unsigned long len, unsigned long off) {
  printf("Invalid character for number: ");

  printcarray(str, off);

  if (isprint(str[off])) {
    printf("[%c]", str[off]);
  } else {
    printf("[0x%x]", str[off]);
  }

  printcarray(str + off + 1, len - off - 1);

  puts("");

  return;
}

int main(int ac, const char** av) {
  reset_progname;
  if (ac != 2) {
    err("Usage: %s <number of tests>", (!ac ? progname : av[0]));

    return 1;
  }

  char* endptr = NULL;
  unsigned long slen = strlen(av[1]);
  long runs = strtol(av[1], &endptr, 0);
  if (endptr < av[1] + slen) {
    print_inval_strtol(av[1], slen, endptr - av[1]);

    return 2;
  }

  if (runs <= 0 || runs > UINT_MAX) {
    fprintf(stderr, "Invalid number of runs `%ld': Valid range is 0 to %u\n", runs, UINT_MAX);

    return 3;
  }

  test_dynamic(runs);
  reset_progname;

  return 0;
}
