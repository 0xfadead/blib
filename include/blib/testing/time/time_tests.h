#ifndef __BLIB_TESTING_TIME_TIME_TESTS_H__
#define __BLIB_TESTING_TIME_TIME_TESTS_H__
#include <time.h>
#include <stdbool.h>

/**
 * @enum TIMETEST_STATE
 * @brief result of a singular time_test
 * @var TIMETEST_STATE::TEST_UNKNOWN
 * The verification step has been skipped
 * @var TIMETEST_STATE::TEST_SUCCESS
 * The test was successful
 * @var TIMETEST_STATE::TEST_FAILURE
 * The test was unsuccessful
 * @var TIMETEST_STATE::TEST_SKIPPED
 * The test was skipped
 */
enum TIMETEST_STATE {
  TEST_UNKNOWN,
  TEST_SUCCESS,
  TEST_FAILURE,
  TEST_SKIPPED,
};

/**
 * @struct time_test
 * @brief A single measurement of time
 * @var time_test::caption
 * The title of the measurement
 * @var time_test::start
 * Time of the start of the measurement
 * @var time_test::end
 * Time of the end of the measurement
 * @var time_test::taken
 * Time delay between start and end
 */
typedef struct {
  const char* caption;
  clock_t start;
  clock_t end;
  clock_t taken;

  enum TIMETEST_STATE state;
} time_test;

/**
 * @function time_test_start
 * @brief Start a singular time test
 * @param caption
 * [in,opt] Name of the test
 */
time_test time_test_start(const char* caption);

/**
 * @function time_test_end
 * @brief End a singular time test
 * @param test
 * [in] The started time test
 */
void time_test_end(time_test *test);

/**
 * @enum time_testrun_features
 * @brief Enum to enable features of a testrun
 * @var time_testrun_features::TESTRUN_ENABLE_TEST_CHECK
 * Enable checking test results
 * @var time_testrun_features::TESTRUN_ENABLE_TIME_TRACK
 * Enable tracking time taken by the tests
 * @var time_testrun_features::TESTRUN_ENABLE_ALL
 * Enable all features
 */
enum time_testrun_features {
  TESTRUN_ENABLE_TEST_CHECK = 1,
  TESTRUN_ENABLE_TIME_TRACK = 1 << 1,
  TESTRUN_ENABLE_ALL = TESTRUN_ENABLE_TEST_CHECK | TESTRUN_ENABLE_TIME_TRACK,
};

/**
 * @struct time_testrun_template
 * @brief A run of tests
 * @var time_testrun_template::caption 
 * Name of the test
 * @var time_testrun_template::num
 * Number of tests to run
 * @var time_testrun_template::testdata
 * Data passed to the test function
 * @var time_testrun_template::testfunc
 * Function called for every test
 * @var time_testrun_template::enable_test_check
 * Enable checking return of the tests
 * @var time_testrun_template::enable_time_tracking
 * Enable time measurement of the tests 
 */
typedef struct {
  const char* caption;
  unsigned long num;
  void* testdata;
  time_test (*testfunc)(unsigned int index, void* data);

  bool enable_test_check;
  bool enable_time_tracking;
} time_testrun_template;

/**
 * @function time_testrun_new
 * @brief Create a new testrun
 * @param caption
 * [in,opt] Name of the testrun
 * @param testnumber
 * [in] number of tests
 * @param testfunc
 * [in,opt] Function called for every test
 * @param data
 * [in,opt] Data passed to the test function
 * @param features
 * [in] Features of the testrun (see `time_testrun_features')
 */
time_testrun_template time_testrun_new(const char* caption, unsigned long testnumber, time_test (*testfunc)(unsigned int index, void* data), void* data, unsigned int features);


/**
 * @struct time_testrun_results
 * @brief Results of a testrun
 * @var caption
 * Name of the testrun
 * @var num
 * Number of tests
 * @var success
 * Number of successful tests
 * @var failure
 * Number of unsuccessful tests
 * @var skipped
 * Number of skipped tests
 * @var min
 * Minimum time taken
 * @var avg
 * Average time taken
 * @var max
 * Maximum time taken
 * @var total
 * Total time taken
 * @var enable_test_check
 * Is test checking enabled
 * @var enable_time_tracking
 * Is time tracking enabled
 */
typedef struct {
  const char* caption;
  unsigned long num;

  unsigned long success;
  unsigned long failure;
  unsigned long skipped;

  clock_t min;
  float   avg;
  clock_t max;
  clock_t total;

  bool enable_test_check;
  bool enable_time_tracking;
} time_testrun_results;

/**
 * @function time_testrun_run
 * @brief Run a testrun
 * @param template
 * [in] Testrun template created by `time_testrun_new()'
 */
time_testrun_results time_testrun_run(const time_testrun_template* template);

/**
 * @function time_testrun_print
 * @brief Print results of a testrun 
 * @param results
 * [in] Results generated by `time_testrun_run()'
 */
void time_testrun_print(const time_testrun_results* results);

#endif // !__BLIB_TESTING_TIME_TIME_TESTS_H__
