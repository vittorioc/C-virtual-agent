#ifndef _SUITE_TEST_LIST_OF_STRING_H
#define	_SUITE_TEST_LIST_OF_STRING_H

int initTestListOfString(void);
int cleanTestListOfString(void);
void test_String_allocList(void);
void test_String_addElementToList_lista_vuota(void);
void test_String_addElementToList_di_due_string_su_lista_vuota(void);
void test_createIterator_lista_vuota(void);
void test_createIterator_lista_un_elemento(void);
void test_freeIterator(void);
void test_hasNext_lista_vuota(void);
void test_hasNext_lista_un_elemento(void);
void test_next_lista_vuota(void);
void test_next_lista_un_elemento(void);
void test_next_lista_due_elementi(void);
void test_scorrimento_lista_molti_elementi(void);
void test_freeList_lista_vuota(void);
void test_freeList_lista_un_elemento(void);
void test_freeList_lista_con_piu_elementi(void);
void test_removeLastReturned_lista_vuota(void);
void test_removeLastReturned_lista_un_elemento(void);
void test_removeLastReturned_primo_di_due_elementi(void);
void test_removeLastReturned_secondo_di_due_elementi(void);
void test_removeLastReturned_primo_di_molti_elementi(void);
void test_removeLastReturned_ultimo_di_molti_elementi(void);
void test_removeLastReturned_in_mezzo_a_lista_di_molti_elementi(void);
void test_removeElement_lista_vuota(void);
void test_removeElement_lista_un_elemento_differente(void);
void test_removeElement_lista_un_elemento_uguale(void);
void test_removeElement_lista_molti_elementi_nessuno_uguale(void);
void test_removeElement_lista_molti_elementi_uno_uguale(void);
void test_removeElement_lista_molti_elementi_piu_di_uno_uguale(void);

#endif	/* _SUITE_TEST_LIST_OF_STRING_H */

