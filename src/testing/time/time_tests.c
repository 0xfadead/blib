#include <blib/testing/time/time_tests.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

time_test time_test_start(const char* caption) {
  time_test test = (time_test) {
    .caption = caption,
    .start = 0,
    .end = 0,
    .taken = 0
  };

  test.start = clock();

  return test;
} /* time_test_start */

void time_test_end(time_test *test) {
  const clock_t t = clock();
  test->end = t;
  test->taken = test->end - test->start;

  return;
} /* time_test_end */

time_testrun_template time_testrun_new(const char* caption, unsigned long testnumber, time_test (*testfunc)(unsigned int index, void* data), void* data, unsigned int features) {
  time_testrun_template template = (time_testrun_template) {
    .caption = caption,
    .num = testnumber,
    .testdata = data,
    .testfunc = testfunc,
  };

  if (features & 1) 
    template.enable_test_check = true;
  if ((features >> 1) & 1)
    template.enable_time_tracking = true;


  return template;
} /* time_testrun_new */

time_testrun_results time_testrun_run(const time_testrun_template* template) {
  time_testrun_results results = {
    .caption = template->caption,
    .num = template->num,

    .enable_test_check = template->enable_test_check,
    .enable_time_tracking = template->enable_time_tracking,

    .max = 0,
    .min = INT_MAX,
    .avg = 0,
    .total = 0,
  };

  for (unsigned int t = 0; t < template->num; t++) {
    time_test test = template->testfunc(t, template->testdata);
    if (!template->enable_time_tracking)
      goto test_check;

    if (test.taken > results.max)
      results.max = test.taken;
    if (test.taken < results.min)
      results.min = test.taken;

    results.total += test.taken;
test_check:
    if (!template->enable_test_check)
      continue;

    switch(test.state) {
      case TEST_UNKNOWN:
        fprintf(stderr, 
            "Test #%u returned invalid state of TEST_UNKNOWN!\n"
            "=== ABORT ===\n",
            t);

        abort();
        break;

      case TEST_SUCCESS:
        results.success++;
        break;

      case TEST_FAILURE:
        results.failure++;
        fprintf(stderr, "%s: #%u: Test failed!\n", template->caption, t);
        break;

      case TEST_SKIPPED:
        results.skipped++;
        break;
    }
  }

  results.avg = (float)results.total / (template->num ? template->num : 1);

  return results;
} /* time_testrun_run */

void time_testrun_print(const time_testrun_results* results) {
  if (!results->enable_time_tracking)
    goto test_check;

  clock_t div = CLOCKS_PER_SEC / 1000000;
  if (!div)
    div = 1;

  printf(
      "%s:\n"
      "\tTIME:\n"
      "\t\tMIN:\t%lu\tmicros\n"
      "\t\tMAX:\t%lu\tmicros\n"
      "\t\tAVG:\t%.2f\tmicros\n"
      "\t\tTOTAL:\t%lu\tmicros in %lu runs\n",
      (results->caption ? results->caption : ""),
      results->min / div,
      results->max / div,
      results->avg / div,
      results->total / div,
      results->num);

test_check:
  if (!results->enable_test_check)
    return;

  printf(
      "\tCHECK:\n"
      "\t\tSUCCESS: %lu\n"
      "\t\tFAILURE: %lu\n"
      "\t\tSKIPPED: %lu\n",
      results->success,
      results->failure,
      results->skipped);

  return;
} /* time_testrun_print */
