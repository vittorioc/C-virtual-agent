#include <pthread.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/CUCurses.h>
#include "list.h"

struct list_element_t {
    element element_pointer;
    struct list_element_t *list_element_prev;
    struct list_element_t *list_element_succ;
};

struct list_t {
    struct list_element_t *list_element_head;
    struct list_element_t *list_element_tail;
    pthread_mutex_t list_mutex;
};

struct list_iterator_t {
    struct list_t *list_pointer;
    struct list_element_t *list_element_next;
};

typedef struct list_element_t *list_element;

int initTestListOfString(void) {

    return 0;
}

int cleanTestListOfString(void) {

    return 0;
}

void test_String_allocList(void) {

    list list_;

    list_ = allocList();

    CU_ASSERT_PTR_NOT_EQUAL(list_, NULL);

    CU_ASSERT_PTR_EQUAL(list_->list_element_tail, NULL);

    CU_ASSERT_PTR_EQUAL(list_->list_element_head, NULL);

    freeList(list_);
}

void test_String_addElementToList_lista_vuota(void) {

    list list_ = allocList();

    addElementToList(list_, "reader");

    CU_ASSERT_PTR_EQUAL(list_->list_element_head->list_element_prev, NULL);

    CU_ASSERT_PTR_EQUAL(list_->list_element_head->element_pointer, "reader");

    CU_ASSERT_PTR_EQUAL(list_->list_element_head->list_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(list_->list_element_tail, list_->list_element_head);

    freeList(list_);

}

void test_String_addElementToList_di_due_string_su_lista_vuota(void) {

    list list_ = allocList();

    addElementToList(list_, "reader3");

    addElementToList(list_, "reader7");

    CU_ASSERT_PTR_EQUAL(list_->list_element_head->list_element_prev, NULL);

    CU_ASSERT_PTR_EQUAL(list_->list_element_head->element_pointer, "reader3");

    CU_ASSERT_PTR_EQUAL(list_->list_element_head->list_element_succ, list_->list_element_tail);

    CU_ASSERT_PTR_EQUAL(list_->list_element_tail->list_element_prev, list_->list_element_head);

    CU_ASSERT_PTR_EQUAL(list_->list_element_tail->element_pointer, "reader7");

    CU_ASSERT_PTR_EQUAL(list_->list_element_tail->list_element_succ, NULL);

    freeList(list_);
}

void test_createIterator_lista_vuota(void) {

    list l = allocList();

    list_iterator iter = allocListIterator(l);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, l->list_element_head);

    CU_ASSERT_PTR_NOT_EQUAL(l, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_head, NULL);

}

void test_createIterator_lista_un_elemento(void) {

    list l;

    list_iterator iter;

    element _element;

    l = allocList();

    _element = (element) "reader5";

    addElementToList(l, _element);

    iter = allocListIterator(l);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, l->list_element_head);

    CU_ASSERT_PTR_NOT_EQUAL(l, NULL);

    CU_ASSERT_STRING_EQUAL(l->list_element_head->element_pointer, _element);

    CU_ASSERT_PTR_EQUAL(l->list_element_head->list_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, l->list_element_head);
}

void test_freeIterator(void) {

    list l;

    list_iterator iter;

    element _element;

    l = allocList();

    _element = (element) "reader5";

    addElementToList(l, _element);

    iter = allocListIterator(l);

    freeListIterator(iter);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, NULL);

    CU_ASSERT_PTR_NOT_EQUAL(l, NULL);

    CU_ASSERT_STRING_EQUAL(l->list_element_head->element_pointer, _element);

    CU_ASSERT_PTR_EQUAL(l->list_element_head->list_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, l->list_element_head);
}

void test_hasNext_lista_vuota(void) {

    list l = allocList();

    list_iterator iter = allocListIterator(l);

    CU_ASSERT_EQUAL(hasNextList(iter), 0);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, l->list_element_head);

    CU_ASSERT_PTR_NOT_EQUAL(l, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_head, NULL);

}

void test_hasNext_lista_un_elemento(void) {

    list l;

    list_iterator iter;

    element _element;

    l = allocList();

    _element = (element) "reader5";

    addElementToList(l, _element);

    iter = allocListIterator(l);

    CU_ASSERT_EQUAL(hasNextList(iter), 1);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, l->list_element_head);

    CU_ASSERT_PTR_NOT_EQUAL(l, NULL);

    CU_ASSERT_STRING_EQUAL(l->list_element_head->element_pointer, _element);

    CU_ASSERT_PTR_EQUAL(l->list_element_head->list_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, l->list_element_head);

}

void test_next_lista_vuota(void) {

    list l = allocList();

    list_iterator iter = allocListIterator(l);

    CU_ASSERT_PTR_EQUAL(nextElementFromList(iter), NULL);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, l->list_element_head);

    CU_ASSERT_PTR_NOT_EQUAL(l, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_head, NULL);

}

void test_next_lista_un_elemento(void) {

    list l;

    list_iterator iter;

    element _element;

    l = allocList();

    _element = (element) "reader8";

    addElementToList(l, _element);

    iter = allocListIterator(l);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, l->list_element_head);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter), _element);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, l->list_element_head->list_element_succ);

    CU_ASSERT_PTR_NOT_EQUAL(l, NULL);

    CU_ASSERT_EQUAL(l->list_element_head->element_pointer, _element);

    CU_ASSERT_PTR_EQUAL(l->list_element_head->list_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, l->list_element_head);

}

void test_next_lista_due_elementi(void) {

    list l;

    list_iterator iter;

    element element1;

    element element2;

    l = allocList();

    element1 = (element) "reader8";

    element2 = (element) "reader8";

    addElementToList(l, element1);

    addElementToList(l, element2);

    iter = allocListIterator(l);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, l->list_element_head);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter), element1);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, l->list_element_head->list_element_succ);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter), element2);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, l->list_element_head->list_element_succ->list_element_succ);

    CU_ASSERT_STRING_EQUAL(l->list_element_head->element_pointer, element1);

    CU_ASSERT_STRING_EQUAL(l->list_element_head->list_element_succ->element_pointer, element2);

    CU_ASSERT_PTR_EQUAL(l->list_element_head->list_element_succ->list_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, l->list_element_head->list_element_succ);

}

void test_scorrimento_lista_molti_elementi(void) {

    list l = allocList();

    element elementi[10] = {"reader8", "reader5", "reader2", "reader6", "reader4", "reader6", "reader16", "reader7", "reader0", "reader0"};

    list_iterator iter;

    int i;

    for (i = 0; i < 10; i++) {

        addElementToList(l, (element) elementi[i]);
    }

    iter = allocListIterator(l);

    i = 0;

    while (hasNextList(iter) == 1) {

        CU_ASSERT_STRING_EQUAL(nextElementFromList(iter), elementi[i]);

        i++;
    }

    CU_ASSERT_EQUAL(hasNextList(iter), 0);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, NULL);

}

void test_freeList_lista_vuota(void) {

    list l = allocList();

    freeList(l);

    CU_ASSERT_PTR_NOT_EQUAL(l, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_head, NULL);
}

void test_freeList_lista_un_elemento(void) {

    list l = allocList();

    element _element = (element) "reader8";

    addElementToList(l, _element);

    freeList(l);

    CU_ASSERT_PTR_NOT_EQUAL(l, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_head, NULL);
}

void test_freeList_lista_con_piu_elementi(void) {

    list l;

    element element1, element2, element3, element4, element5;

    l = allocList();

    element1 = (element) "reader8";

    element2 = (element) "reader3";

    element3 = (element) "reader2";

    element4 = (element) "reader1";

    element5 = (element) "reader7";

    addElementToList(l, element1);

    addElementToList(l, element2);

    addElementToList(l, element3);

    addElementToList(l, element4);

    addElementToList(l, element5);

    freeList(l);

    CU_ASSERT_PTR_NOT_EQUAL(l, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_head, NULL);
}

void test_removeLastReturned_lista_vuota(void) {

    list l = allocList();

    list_iterator iter = allocListIterator(l);

    removeLastReturnedElementFromList(iter);

    CU_ASSERT_PTR_NOT_EQUAL(l, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_head, NULL);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, l->list_element_head);

}

void test_removeLastReturned_lista_un_elemento(void) {

    list l = allocList();

    element _element = (element) "reader9";

    list_iterator iter;

    addElementToList(l, _element);

    iter = allocListIterator(l);

    CU_ASSERT_PTR_NOT_EQUAL(l, NULL);

    CU_ASSERT_STRING_EQUAL(l->list_element_head->element_pointer, _element);

    CU_ASSERT_PTR_EQUAL(l->list_element_head->list_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, l->list_element_head);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, l->list_element_head);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter), _element);

    removeLastReturnedElementFromList(iter);

    CU_ASSERT_PTR_NOT_EQUAL(l, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_head, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, NULL);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, NULL);

}

void test_removeLastReturned_primo_di_due_elementi(void) {

    list l = allocList();

    element element1 = (element) "reader9";

    element element2 = (element) "reader5";

    list_iterator iter;

    addElementToList(l, element1);

    addElementToList(l, element2);

    iter = allocListIterator(l);

    CU_ASSERT_PTR_NOT_EQUAL(l, NULL);

    CU_ASSERT_STRING_EQUAL(l->list_element_head->element_pointer, element1);

    CU_ASSERT_STRING_EQUAL(l->list_element_head->list_element_succ->element_pointer, element2);

    CU_ASSERT_PTR_EQUAL(l->list_element_head->list_element_succ->list_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, l->list_element_head->list_element_succ);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, l->list_element_head);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter), element1);

    removeLastReturnedElementFromList(iter);

    CU_ASSERT_PTR_NOT_EQUAL(l, NULL);

    CU_ASSERT_STRING_EQUAL(l->list_element_head->element_pointer, element2);

    CU_ASSERT_PTR_EQUAL(l->list_element_head->list_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, l->list_element_head);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, l->list_element_head);

}

void test_removeLastReturned_secondo_di_due_elementi(void) {

    list l = allocList();

    element element1 = (element) "reader9";

    element element2 = (element) "reader5";

    list_iterator iter;

    addElementToList(l, element1);

    addElementToList(l, element2);

    iter = allocListIterator(l);

    CU_ASSERT_PTR_NOT_EQUAL(l, NULL);

    CU_ASSERT_STRING_EQUAL(l->list_element_head->element_pointer, element1);

    CU_ASSERT_STRING_EQUAL(l->list_element_head->list_element_succ->element_pointer, element2);

    CU_ASSERT_PTR_EQUAL(l->list_element_head->list_element_succ->list_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, l->list_element_head->list_element_succ);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, l->list_element_head);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter), element1);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter), element2);

    removeLastReturnedElementFromList(iter);

    CU_ASSERT_PTR_NOT_EQUAL(l, NULL);

    CU_ASSERT_STRING_EQUAL(l->list_element_head->element_pointer, element1);

    CU_ASSERT_PTR_EQUAL(l->list_element_head->list_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, l->list_element_head);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, NULL);

}

void test_removeLastReturned_primo_di_molti_elementi(void) {

    list l = allocList();

    element lista_inizio[10] = {"reader8", "reader5", "reader2", "reader6", "reader4", "reader6", "reader16", "reader7", "reader0", "reader0"};

    element lista_fine[9] = {"reader5", "reader2", "reader6", "reader4", "reader6", "reader16", "reader7", "reader0", "reader0"};

    list_iterator iter1, iter2;

    int i;

    for (i = 0; i < 10; i++) {

        addElementToList(l, (element) lista_inizio[i]);
    }

    iter1 = allocListIterator(l);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter1), lista_inizio[0]);

    removeLastReturnedElementFromList(iter1);

    iter2 = allocListIterator(l);

    i = 0;

    while (hasNextList(iter2) == 1) {

        CU_ASSERT_STRING_EQUAL(nextElementFromList(iter2), lista_fine[i]);

        i++;
    }

    CU_ASSERT_EQUAL(hasNextList(iter2), 0);

    CU_ASSERT_PTR_NOT_EQUAL(iter2, NULL);

    CU_ASSERT_PTR_EQUAL(iter2->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter2->list_element_next, NULL);

}

void test_removeLastReturned_ultimo_di_molti_elementi(void) {

    list l = allocList();

    element lista_inizio[10] = {"reader8", "reader5", "reader2", "reader6", "reader4", "reader6", "reader16", "reader7", "reader0", "reader5"};

    element lista_fine[9] = {"reader8", "reader5", "reader2", "reader6", "reader4", "reader6", "reader16", "reader7", "reader0"};

    list_iterator iter1, iter2;

    int i;

    for (i = 0; i < 10; i++) {

        addElementToList(l, (element) lista_inizio[i]);
    }

    iter1 = allocListIterator(l);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter1), lista_inizio[0]);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter1), lista_inizio[1]);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter1), lista_inizio[2]);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter1), lista_inizio[3]);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter1), lista_inizio[4]);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter1), lista_inizio[5]);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter1), lista_inizio[6]);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter1), lista_inizio[7]);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter1), lista_inizio[8]);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter1), lista_inizio[9]);

    removeLastReturnedElementFromList(iter1);

    iter2 = allocListIterator(l);

    i = 0;

    while (hasNextList(iter2) == 1) {

        CU_ASSERT_STRING_EQUAL(nextElementFromList(iter2), lista_fine[i]);

        i++;
    }

    CU_ASSERT_EQUAL(hasNextList(iter2), 0);

    CU_ASSERT_PTR_NOT_EQUAL(iter2, NULL);

    CU_ASSERT_PTR_EQUAL(iter2->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter2->list_element_next, NULL);
}

void test_removeLastReturned_in_mezzo_a_lista_di_molti_elementi(void) {

    list l = allocList();

    element lista_inizio[10] = {"reader8", "reader5", "reader2", "reader6", "reader4", "reader6", "reader16", "reader7", "reader0", "reader5"};

    element lista_fine[9] = {"reader8", "reader5", "reader2", "reader6", "reader4", "reader16", "reader7", "reader0", "reader5"};

    list_iterator iter1, iter2;

    int i;

    for (i = 0; i < 10; i++) {

        addElementToList(l, (element) lista_inizio[i]);
    }

    iter1 = allocListIterator(l);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter1), lista_inizio[0]);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter1), lista_inizio[1]);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter1), lista_inizio[2]);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter1), lista_inizio[3]);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter1), lista_inizio[4]);

    CU_ASSERT_STRING_EQUAL(nextElementFromList(iter1), lista_inizio[5]);

    removeLastReturnedElementFromList(iter1);

    iter2 = allocListIterator(l);

    i = 0;

    while (hasNextList(iter2) == 1) {

        CU_ASSERT_STRING_EQUAL(nextElementFromList(iter2), lista_fine[i]);

        i++;
    }

    CU_ASSERT_EQUAL(hasNextList(iter2), 0);

    CU_ASSERT_PTR_NOT_EQUAL(iter2, NULL);

    CU_ASSERT_PTR_EQUAL(iter2->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter2->list_element_next, NULL);
}

void test_removeElement_lista_vuota(void) {

    list l = allocList();

    element _element = (element) "reader9";

    removeElementFromList(l, _element);

    CU_ASSERT_PTR_NOT_EQUAL(l, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_head, NULL);

}

void test_removeElement_lista_un_elemento_differente(void) {

    list l = allocList();

    element element1 = (element) "reader9";

    element element2 = (element) "reader4";

    addElementToList(l, element1);

    CU_ASSERT_STRING_EQUAL(l->list_element_head->element_pointer, element1);

    CU_ASSERT_PTR_EQUAL(l->list_element_head->list_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, l->list_element_head);

    removeElementFromList(l, element2);

    CU_ASSERT_STRING_EQUAL(l->list_element_head->element_pointer, element1);

    CU_ASSERT_PTR_EQUAL(l->list_element_head->list_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, l->list_element_head);

}

void test_removeElement_lista_un_elemento_uguale(void) {

    list l = allocList();

    element _element = (element) "reader9";

    addElementToList(l, _element);

    CU_ASSERT_STRING_EQUAL(l->list_element_head->element_pointer, _element);

    CU_ASSERT_PTR_EQUAL(l->list_element_head->list_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, l->list_element_head);

    removeElementFromList(l, _element);

    CU_ASSERT_PTR_NOT_EQUAL(l, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_tail, NULL);

    CU_ASSERT_PTR_EQUAL(l->list_element_head, NULL);
}

void test_removeElement_lista_molti_elementi_nessuno_uguale(void) {

    list l = allocList();

    element elementi[10] = {"reader8", "reader5", "reader2", "reader6", "reader4", "reader6", "reader16", "reader7", "reader0", "reader0"};

    element _element = (element) "reader9";

    list_iterator iter;

    int i;

    for (i = 0; i < 10; i++) {

        addElementToList(l, (element) elementi[i]);
    }

    removeElementFromList(l, _element);

    iter = allocListIterator(l);

    i = 0;

    while (hasNextList(iter) == 1) {

        CU_ASSERT_STRING_EQUAL(nextElementFromList(iter), elementi[i]);

        i++;
    }

    CU_ASSERT_EQUAL(hasNextList(iter), 0);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, NULL);

}

void test_removeElement_lista_molti_elementi_uno_uguale(void) {

    list l = allocList();

    element lista_inizio[10] = {"reader8", "reader5", "reader2", "reader6", "reader4", "reader6", "reader16", "reader7", "reader0", "reader0"};

    element lista_fine[9] = {"reader8", "reader5", "reader6", "reader4", "reader6", "reader16", "reader7", "reader0", "reader0"};

    element _element = (element) "reader2";

    list_iterator iter;

    int i;

    for (i = 0; i < 10; i++) {

        addElementToList(l, (element) lista_inizio[i]);
    }

    removeElementFromList(l, _element);

    iter = allocListIterator(l);

    i = 0;

    while (hasNextList(iter) == 1) {

        CU_ASSERT_STRING_EQUAL(nextElementFromList(iter), lista_fine[i]);

        i++;
    }

    CU_ASSERT_EQUAL(hasNextList(iter), 0);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, NULL);
}

void test_removeElement_lista_molti_elementi_piu_di_uno_uguale(void) {

    list l = allocList();

    element lista_inizio[10] = {"reader8", "reader5", "reader2", "reader6", "reader4", "reader6", "reader16", "reader7", "reader0", "reader0"};

    element lista_fine[9] = {"reader8", "reader5", "reader2", "reader4", "reader6", "reader16", "reader7", "reader0", "reader0"};

    element _element = (element) "reader6";

    list_iterator iter;

    int i;

    for (i = 0; i < 10; i++) {

        addElementToList(l, (element) lista_inizio[i]);
    }

    removeElementFromList(l, _element);

    iter = allocListIterator(l);

    i = 0;

    while (hasNextList(iter) == 1) {

        CU_ASSERT_EQUAL(nextElementFromList(iter), lista_fine[i]);

        i++;
    }

    CU_ASSERT_EQUAL(hasNextList(iter), 0);

    CU_ASSERT_PTR_NOT_EQUAL(iter, NULL);

    CU_ASSERT_PTR_EQUAL(iter->list_pointer, l);

    CU_ASSERT_PTR_EQUAL(iter->list_element_next, NULL);
}