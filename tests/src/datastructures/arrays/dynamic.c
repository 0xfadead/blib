#include <blib/datastructures/arrays/dynamic.h>
#include <blib/testing/testing.h>
#include <stdio.h>
#include <time.h>
#include <util.h>

time_test test_creation(unsigned int index, void* data) {
  (void)index;
  (void)data;

  time_test test = time_test_start(NULL);
  dynamic_arr arr = dynamic_arr_new(unsigned int);
  time_test_end(&test);
  dynamic_arr_cleanup(&arr);

  return test;
}

void testrun_creation(unsigned int runs) {
  info("Testing dynamic array creation...");
  time_testrun_template creation_template = time_testrun_new("Creation", runs, test_creation, NULL, TESTRUN_ENABLE_TIME_TRACK);
  time_testrun_results creation_res = time_testrun_run(&creation_template);
  time_testrun_print(&creation_res);
  puts("");

  return;
}

time_test test_append(unsigned int index, void* data) {
  dynamic_arr* arr = (dynamic_arr*) data;

  time_test test = time_test_start(NULL);
  dynamic_arr_append(arr, &index);
  time_test_end(&test);

  return test;
}

void testrun_append(unsigned int runs, dynamic_arr* arr) {
  info("Testing dynamic array appending...");
  time_testrun_template append_template = time_testrun_new("Appending", runs, test_append, arr, TESTRUN_ENABLE_TIME_TRACK);
  time_testrun_results append_res = time_testrun_run(&append_template);
  time_testrun_print(&append_res);
  puts("");
}

time_test test_precate(unsigned int index, void* data) {
  dynamic_arr* arr = (dynamic_arr*) data;

  unsigned int out = 0;
  time_test test = time_test_start(NULL);
  dynamic_arr_precate(arr, &out);
  time_test_end(&test);
  if (out != index)
    test.state = TEST_FAILURE;
  else
    test.state = TEST_SUCCESS;

  return test;
}

void testrun_precate(unsigned int runs, dynamic_arr* arr) {
  info("Testing dynamic array precating...");
  time_testrun_template precate_template = time_testrun_new("Precating", runs, test_precate, arr, TESTRUN_ENABLE_ALL);
  time_testrun_results precate_res = time_testrun_run(&precate_template);
  time_testrun_print(&precate_res);
  puts("");

  return;
}

struct test_quick {
  unsigned int off;
  dynamic_arr* arr;
};

time_test test_quick_precate(unsigned int index, void* data) {
  struct test_quick* quick = (struct test_quick*) data;

  unsigned int out = 0;
  time_test test = time_test_start(NULL);
  dynamic_arr_quick_precate(quick->arr, &out);
  time_test_end(&test);

  if (out != index + quick->off)
    test.state = TEST_FAILURE;
  else
    test.state = TEST_SUCCESS;

  return test;
}

void testrun_quick_precate(unsigned int runs, unsigned int off, dynamic_arr* arr) {
  info("Testing dynamic array quick precating...");
  time_testrun_template precate_quick_template = time_testrun_new("Quick Precating", runs, test_quick_precate, &(struct test_quick){.arr = arr, .off = off}, TESTRUN_ENABLE_ALL);
  time_testrun_results precate_quick_res = time_testrun_run(&precate_quick_template);
  time_testrun_print(&precate_quick_res);
  puts("");

  return;
}

test_results test_dynamic(unsigned int runs) {
  test_results res = {0};
  (void) runs;
  set_prog_name("dynamic");


  testrun_creation(runs);
  dynamic_arr main = dynamic_arr_new(unsigned int);

  testrun_append(runs, &main);

  unsigned long precate = runs >> 1;
  unsigned long qprecate = runs - precate;

  testrun_precate(precate, &main);

  testrun_quick_precate(qprecate, qprecate, &main);

  info("Testing dynamic array trimming...");
  time_test trim_test = time_test_start(NULL);
  dynamic_arr_trim(&main);
  time_test_end(&trim_test);
  printf("Trim time:\t%ld micros\n", trim_test.taken / (CLOCKS_PER_SEC / 1000000));

  dynamic_arr_append(NULL, NULL);

  dynamic_arr_cleanup(&main);
  set_prog_name(NULL);
  return res;
}
