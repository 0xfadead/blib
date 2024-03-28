# Blib

-------

## About

**B**ase **Lib**rary (or Blib) is a collection of basic functions and datastructure implementations.

## Usage

<details closed>
<summary>Dynamic arrays</summary>

```c
#include <blib/datastructures/arrays/dynamic.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    dynamic_arr* arr = dynamic_arr_new(int);

    /* Append the elements */
    int elem = 1;
    dynamic_arr_append(arr, &elem);
    elem = 3;
    dynamic_arr_append(arr, &elem);
    elem = 2;
    dynamic_arr_insert_at(arr, 1, &elem);
    /* ------------------- */

    /* retrieve three elements */
    int elems[3] = {0};
    dynamic_arr_bulk_peek(arr, 0, elems, 2);
    /* --------------------- */

    /*  */
    printf(
        "elems[0] = %i; elems[1] = %i; elems[2] = %i\n", 
        elems[0], elems[1], elems[2]);

    dynamic_arr_truncate(arr, NULL);

    dynamic_arr_cleanup(arr);

    return 0;
}
```
</details>

<details closed>
<summary>Testing</summary>
```c
#include <blib/testing/time/time_tests.h>
#include <stdlib.h>

time_test testfunc(unsigned int index, void* data) {
    (void)data;

    time_test test = time_test_start(NULL);

    volatile unsigned char i = rand() & 255;

    time_test_end(&test);

    test.state = (i != 69 ? TEST_SUCCESS : TEST_FAILURE);

    return test;
}

int main(void) {
    time_testrun_template testrun = time_testrun_new(
                                    "Random test", 
                                    1024, testfunc, NULL, 
                                    TESTRUN_ENABLE_ALL);

    time_testrun_results results = time_testrun_run(&testrun);

    time_testrun_print(&results);

    return 0;
}    
```
</details>
