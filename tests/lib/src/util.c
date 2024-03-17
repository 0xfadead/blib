#include <util.h>

#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>

const char* progname = NULL;

void set_prog_name(const char* name) {
  progname = name;

  return;
}

void __intern_log_base(FILE* file, const char* prefix, const char* srcfile, int srcline, const char* fmt, ...) {
  (void)srcfile;
  (void)srcline;
  va_list argp;

  va_start(argp, fmt);

  fprintf(file, "%s: %s ", (progname ? progname : "unknown"), prefix);
  vfprintf(file, fmt, argp);
  fputs("\n", file);

  va_end(argp);

  return;
}

void printcarray(const char* str, unsigned long len) {
  if (!len) 
    return;

  for (unsigned long i = 0; i < len - 1; i++)
    printf("%c ", str[i]);
  printf("%c", str[len - 1]);

  return;
}
