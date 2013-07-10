#define _REENTRANT

#include <stdio.h>
#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/CUCurses.h>
#include "suite_test_lab.h"

int main(int argc, char** argv) {

    CU_pSuite suite_test_lab = NULL;

    argc = 0;
    argv = NULL;

    /*  initialize the CUnit test registry */

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    if (NULL == (suite_test_lab = CU_add_suite("lab", initTestLab, cleanTestLab))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
            (NULL == CU_add_test(suite_test_lab, "test1                                                      ", test_lab))
            ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /*
     * Run all tests using the basic interface
     *
     * CU_basic_run_tests(); -> Runs all tests in all registered suites
     * CU_basic_run_suite(CU_pSuite pSuite); -> Runs all tests in single specified suite
     * CU_basic_run_test(CU_pSuite pSuite, CU_pTest pTest); -> Runs a single test in a specified suite
     */

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    printf("\n");
    CU_basic_show_failures(CU_get_failure_list());
    printf("\n\n");

    /*
     * Run all tests using the automated interface
     */

    /*
    CU_automated_run_tests();
    CU_list_tests_to_file();
     */

    /*
     * Run all tests using the console interface
     */

    /*
        CU_console_run_tests();
     */

    /*
     * Run all tests using the curses interface
     * (only on systems having curses)
     */

    /*
        CU_curses_run_tests();
     */

    /*    Clean up registry and return */

    CU_cleanup_registry();
    return CU_get_error();

    return (EXIT_SUCCESS);
}

