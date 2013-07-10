#define _REENTRANT

#include <stdio.h>
#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/CUCurses.h>
#include "suite_test_list_of_agent.h"
#include "suite_test_agent.h"
#include "suite_test_router.h"

int main(int argc, char** argv) {

    CU_pSuite suite_test_list_of_agent = NULL;
    CU_pSuite suite_test_agent = NULL;
    CU_pSuite suite_test_router = NULL;

    argc = 0;
    argv = NULL;

    /*  initialize the CUnit test registry */

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    
    if (NULL == (suite_test_list_of_agent = CU_add_suite("List-agent", initTestListOfAgent, cleanTestListOfAgent))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
            (NULL == CU_add_test(suite_test_list_of_agent, "allocazione di una nuova network                           ", test_Agent_allocList)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "aggiunta di un agent ad una network vuota                  ", test_Agent_addElementToList_lista_vuota)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "aggiunta di due agent ad una network                       ", test_Agent_addElementToList_di_due_agent_su_lista_vuota)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "createIterator network vuota                               ", test_allocNetworkIterator_network_vuota)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "freeIterator network vuota                                 ", test_freeNetworkIterator_network_vuota)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "createIterator network con un agent                        ", test_allocNetworkIterator_network_un_agent)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "freeIterator network con un agent                          ", test_freeNetworkIterator_network_un_agent)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "hasNext network vuota                                      ", test_hasNext_network_vuota)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "hasNext network un agent                                   ", test_hasNext_network_un_agent)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "next network vuota                                         ", test_next_network_vuota)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "next network un agent                                      ", test_next_network_un_agent)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "next network di due agent                                  ", test_next_network_due_agent)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "next di tutti gli agent della network                      ", test_next_network_tutti_gli_agent)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "removeLastReturned di una network vuota                    ", test_removeLastReturned_network_vuota)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "removeLastReturned di una network con un agent             ", test_removeLastReturned_network_un_agent)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "removeLastReturned di una network primo agent di due       ", test_removeLastReturned_primo_di_due_agent)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "removeLastReturned di una network secondo agent di due     ", test_removeLastReturned_secondo_di_due_agent)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "removeLastReturned primo agent di molti inseriti           ", test_removeLastReturned_primo_di_molti_agent)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "removeLastReturned ultimo agent di molti inseriti          ", test_removeLastReturned_ultimo_di_molti_agent)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "removeLastReturned di un agent in mezzo a molti            ", test_removeLastReturned_in_mezzo_a_molti_agent)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "removeAgent - network vuota                                ", test_removeAgent_network_vuota)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "removeAgent - network di un agent differente da quello dato", test_removeAgent_network_un_agent_differente)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "removeAgent - network di un agent uguale a quello dato     ", test_removeAgent_network_un_agent_uguale)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "removeAgent - network molti agent - nessuno uguale         ", test_removeAgent_network_molti_agent_nessuno_uguale)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "removeAgent - network molti agent - uno uguale             ", test_removeAgent_network_molti_agent_uno_uguale)) ||
            (NULL == CU_add_test(suite_test_list_of_agent, "removeAgent - network molti agent - uno inserito più volte ", test_removeAgent_network_molti_agent_uno_ripetuto))
            ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == (suite_test_agent = CU_add_suite("agent", initTestAgent, cleanTestAgent))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

     if (
            (NULL == CU_add_test(suite_test_agent, "test1                                                      ", test_agent))
            ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == (suite_test_router = CU_add_suite("router", initTestRouter, cleanTestRouter))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
            (NULL == CU_add_test(suite_test_router, "test1                                                      ", test_router))
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

