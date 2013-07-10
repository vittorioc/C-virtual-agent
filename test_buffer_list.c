#define _REENTRANT

#include <stdio.h>
#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/CUCurses.h>
#include "suite_test_buffer_of_int.h"
#include "suite_test_list_of_string.h"

int main(int argc, char** argv) {

    CU_pSuite suite_test_buffer_of_int = NULL;
    CU_pSuite suite_test_list_of_string = NULL;

    argc = 0;
    argv = NULL;

    /*  initialize the CUnit test registry */

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    if (NULL == (suite_test_buffer_of_int = CU_add_suite("Buffer-int", initTestBuffer, cleanTestBuffer))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
            (NULL == CU_add_test(suite_test_buffer_of_int, "put non bloccante - buffer vuoto                           ", test_put_non_bloccante_buffer_vuoto)) ||
            (NULL == CU_add_test(suite_test_buffer_of_int, "put bloccante - buffer vuoto                               ", test_put_bloccante_buffer_vuoto)) ||
            (NULL == CU_add_test(suite_test_buffer_of_int, "get non bloccante - buffer pieno                           ", test_get_non_bloccante_buffer_pieno)) ||
            (NULL == CU_add_test(suite_test_buffer_of_int, "get bloccante - buffer pieno                               ", test_get_bloccante_buffer_pieno)) ||
            (NULL == CU_add_test(suite_test_buffer_of_int, "put non bloccante - buffer pieno                           ", test_put_non_bloccante_buffer_pieno)) ||
            (NULL == CU_add_test(suite_test_buffer_of_int, "put bloccante - buffer pieno                               ", test_put_bloccante_buffer_pieno)) ||
            (NULL == CU_add_test(suite_test_buffer_of_int, "get non bloccante -  buffer vuoto                          ", test_get_non_bloccante_buffer_vuoto)) ||
            (NULL == CU_add_test(suite_test_buffer_of_int, "get bloccante - buffer vuoto                               ", test_get_bloccante_buffer_vuoto)) ||
            (NULL == CU_add_test(suite_test_buffer_of_int, "prima un consumatore poi un produttore concorrenti         ", test_prima_un_consumatore_poi_un_produttore_concorrenti)) ||
            (NULL == CU_add_test(suite_test_buffer_of_int, "prima un produttore poi un consumatore concorrenti         ", test_prima_un_produttore_poi_un_consumatore_concorrenti)) ||
            (NULL == CU_add_test(suite_test_buffer_of_int, "molti produttori su buffer inizialmente vuoto              ", test_molti_produttori_su_buffer_inizialmente_vuoto)) ||
            (NULL == CU_add_test(suite_test_buffer_of_int, "molti consumatori su buffer inizialmente pieno             ", test_molti_consumatori_su_buffer_inizialmente_pieno))
            ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == (suite_test_list_of_string = CU_add_suite("List-string", initTestListOfString, cleanTestListOfString))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
            (NULL == CU_add_test(suite_test_list_of_string, "allocazione di una nuova lista                             ", test_String_allocList)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "aggiunta di un elemento ad una lista vuota                 ", test_String_addElementToList_lista_vuota)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "aggiunta di due elementi ad una lista                      ", test_String_addElementToList_di_due_string_su_lista_vuota)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "createIterator lista vuota                                 ", test_createIterator_lista_vuota)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "createIterator lista un elemento                           ", test_createIterator_lista_un_elemento)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "freeIterator                                               ", test_freeIterator)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "hasNext lista vuota                                        ", test_hasNext_lista_vuota)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "hasNext lista un elemento                                  ", test_hasNext_lista_un_elemento)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "next lista vuota                                           ", test_next_lista_vuota)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "next lista un elemento                                     ", test_next_lista_un_elemento)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "next lista due elementi                                    ", test_next_lista_due_elementi)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "scorrimento lista con molti elementi                       ", test_scorrimento_lista_molti_elementi)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "freeList di una lista vuota                                ", test_freeList_lista_vuota)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "freeList di una lista con un elemento                      ", test_freeList_lista_un_elemento)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "freeList di una lista con molti elementi                   ", test_freeList_lista_con_piu_elementi)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "removeLastReturned di una lista vuota                      ", test_removeLastReturned_lista_vuota)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "removeLastReturned di una lista con un elemento            ", test_removeLastReturned_lista_un_elemento)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "removeLastReturned primo elemento di una lista di due      ", test_removeLastReturned_primo_di_due_elementi)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "removeLastReturned secondo elemento di una lista di due    ", test_removeLastReturned_secondo_di_due_elementi)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "removeLastReturned primo elemento di una lista di molti    ", test_removeLastReturned_primo_di_molti_elementi)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "removeLastReturned ultimo elemento di una lista di molti   ", test_removeLastReturned_ultimo_di_molti_elementi)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "removeLastReturned elemento in mezzo a una lista di molti  ", test_removeLastReturned_in_mezzo_a_lista_di_molti_elementi)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "removeElement - lista vuota                                ", test_removeElement_lista_vuota)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "removeElement - lista di un elemento - nessuno uguale      ", test_removeElement_lista_un_elemento_differente)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "removeElement - lista di un elemento uguale                ", test_removeElement_lista_un_elemento_uguale)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "removeElement - lista molti elementi - nessuno uguale      ", test_removeElement_lista_molti_elementi_nessuno_uguale)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "removeElement - lista molti elementi - uno uguale          ", test_removeElement_lista_molti_elementi_uno_uguale)) ||
            (NULL == CU_add_test(suite_test_list_of_string, "removeElement - lista molti elementi - più di uno uguale   ", test_removeElement_lista_molti_elementi_piu_di_uno_uguale))
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

