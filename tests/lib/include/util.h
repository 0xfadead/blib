#ifndef __UTIL_H__
#define __UTIL_H__
#include <stdio.h>

#ifndef TESTS_INFO_PREFIX
#define TESTS_INFO_PREFIX "[INFO]"
#endif

#ifndef TESTS_INFO_FILE
#define TESTS_INFO_FILE stdout
#endif

#ifndef TESTS_WARN_PREFIX
#define TESTS_WARN_PREFIX "[WARN]"
#endif

#ifndef TESTS_WARN_FILE
#define TESTS_WARN_FILE stderr
#endif

#ifndef TESTS_ERR_PREFIX
#define TESTS_ERR_PREFIX "[ERR ]"
#endif

#ifndef TESTS_ERR_FILE
#define TESTS_ERR_FILE stderr
#endif

/**
 * @brief Set program name 
 * @param name Name of the program
 */
void set_prog_name(const char* name);

void __intern_log_base(FILE* file, const char* prefix, const char* srcfile, int srcline, const char* fmt, ...);

/**
 * @brief Log information to TESTS_INFO_FILE with TESTS_INFO_PREFIX
 */
#define info(...) (__intern_log_base(TESTS_INFO_FILE, TESTS_INFO_PREFIX, __FILE__, __LINE__, __VA_ARGS__))
/**
 * @brief Log warnings to TESTS_WARN_FILE with TESTS_WARN_PREFIX
 */
#define warn(...) (__intern_log_base(TESTS_WARN_FILE, TESTS_WARN_PREFIX, __FILE__, __LINE__, __VA_ARGS__))
/**
 * @brief Log error to TESTS_ERR_FILE with TESTS_ERR_PREFIX
 */
#define err(...)  (__intern_log_base(TESTS_ERR_FILE,  TESTS_ERR_PREFIX,  __FILE__, __LINE__, __VA_ARGS__))

typedef struct {
  unsigned int success;
  unsigned int failure;
  unsigned int skipped;
} test_results;

/**
 * @brief Print array of characters with spaces between each character 
 * @param str Array of chars
 * @param len Length of array
 */
void printcarray(const char* str, unsigned long len);

#endif // !__UTIL_H__
