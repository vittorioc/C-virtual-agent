#include <pthread.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/CUCurses.h>
#include "buffer.h"
#include "suite_test_buffer_of_int.h"

struct buffer_cell_t {
    void *cell_pointer;
    struct buffer_cell_t *buffer_cell_succ;
};

struct list_cell_t {
    struct buffer_cell_t *buffer_cell_head;
    struct buffer_cell_t *buffer_cell_tail;
    pthread_mutex_t list_cell_mutex;
    pthread_cond_t list_cell_cond;
};

struct buffer_t {
    int b_id_azione;
    struct list_cell_t *list_cell_empty;
    struct list_cell_t *list_cell_full;
    pthread_mutex_t buffer_mutex;
};

typedef struct buffer_cell_t *buffer_cell;
typedef struct list_cell_t *list_cell;

int numero_produttori_attivi, numero_consumatori_attivi;

int initTestBuffer(void){

    return 0;
}

int cleanTestBuffer(void){

    return 0;
}

void test_put_non_bloccante_buffer_vuoto(void) {

    buffer buf = allocBuffer(1);

    void *messaggio = (void *) 5;

    CU_ASSERT_EQUAL(putInBuffer_not_blocking(buf, messaggio), BUFFER_OK);

    CU_ASSERT_EQUAL(buf->list_cell_full->buffer_cell_head->cell_pointer, messaggio);

    freeBuffer(buf);
}

void test_put_bloccante_buffer_vuoto(void) {

    buffer buf = allocBuffer(1);

    void *messaggio = (void *) 7;

    putInBuffer_blocking(buf, messaggio);

    CU_ASSERT_EQUAL(buf->list_cell_full->buffer_cell_head->cell_pointer, messaggio);

    freeBuffer(buf);
}

void test_get_non_bloccante_buffer_pieno(void) {

    buffer buf = allocBuffer(1);

    void *messaggio = (void *) 9;

    putInBuffer_blocking(buf, messaggio);

    CU_ASSERT_EQUAL(buf->list_cell_full->buffer_cell_head->cell_pointer, messaggio);

    CU_ASSERT_EQUAL(getFromBuffer_not_blocking(buf), messaggio);

    /*
     * Per verificare che il buffer sia vuoto devo controllare che ls lista piene sia vuota
     */

    CU_ASSERT_PTR_EQUAL(buf->list_cell_full->buffer_cell_head, NULL);

    freeBuffer(buf);
}

void test_get_bloccante_buffer_pieno(void) {

    buffer buf = allocBuffer(1);

    void *messaggio = (void *) 23;

    putInBuffer_blocking(buf, messaggio);

    CU_ASSERT_EQUAL(buf->list_cell_full->buffer_cell_head->cell_pointer, messaggio);

    CU_ASSERT_EQUAL(getFromBuffer_blocking(buf), messaggio);

    CU_ASSERT_PTR_EQUAL(buf->list_cell_full->buffer_cell_head, NULL);

    freeBuffer(buf);
}

void test_put_non_bloccante_buffer_pieno(void) {

    buffer buf = allocBuffer(1);

    void *primo_messaggio = (void *) 2;

    void *secondo_messaggio = (void *) 3;

    putInBuffer_blocking(buf, primo_messaggio);

    CU_ASSERT_EQUAL(buf->list_cell_full->buffer_cell_head->cell_pointer, primo_messaggio);

    CU_ASSERT_EQUAL(putInBuffer_not_blocking(buf, secondo_messaggio), BUFFER_PIENO);

    CU_ASSERT_EQUAL(buf->list_cell_full->buffer_cell_head->cell_pointer, primo_messaggio);

    freeBuffer(buf);
}

void* produttore_bloccante_p1(void *p) {

    void *messaggio = (void *) 12;

    buffer buf = (buffer) p;

    putInBuffer_blocking(buf, messaggio);

    numero_produttori_attivi--;

    return NULL;

}

void test_put_bloccante_buffer_pieno(void) {

    pthread_t prod;

    buffer buf = allocBuffer(1);

    void *primo_messaggio = (void *) 8;

    void *secondo_messaggio = (void *) 12;

    putInBuffer_blocking(buf, primo_messaggio);

    CU_ASSERT_EQUAL(buf->list_cell_full->buffer_cell_head->cell_pointer, primo_messaggio);

    numero_produttori_attivi = 1;

    pthread_create(&prod, NULL, &produttore_bloccante_p1, buf);

    getFromBuffer_blocking(buf);

    pthread_join(prod, NULL);

    CU_ASSERT_EQUAL(numero_produttori_attivi, 0);

    CU_ASSERT_EQUAL(buf->list_cell_full->buffer_cell_head->cell_pointer, secondo_messaggio);

    freeBuffer(buf);
}

void test_get_non_bloccante_buffer_vuoto(void) {

    buffer buf = allocBuffer(1);

    CU_ASSERT_EQUAL(getFromBuffer_not_blocking(buf), (void *) BUFFER_VUOTO);

    freeBuffer(buf);
}

void* consumatore_bloccante_c1(void *c) {

    buffer buf = (buffer) c;

    getFromBuffer_blocking(buf);

    numero_consumatori_attivi--;

    return NULL;
}

void test_get_bloccante_buffer_vuoto(void) {

    pthread_t cons;

    buffer buf = allocBuffer(1);

    void *messaggio = (void *) 32;

    numero_consumatori_attivi = 1;

    pthread_create(&cons, NULL, &consumatore_bloccante_c1, buf);

    putInBuffer_blocking(buf, messaggio);

    pthread_join(cons, NULL);

    CU_ASSERT_EQUAL(numero_consumatori_attivi, 0);

    CU_ASSERT_PTR_EQUAL(buf->list_cell_full->buffer_cell_head, NULL);

    freeBuffer(buf);
}

void test_prima_un_consumatore_poi_un_produttore_concorrenti(void) {

    pthread_t cons;
    pthread_t prod;

    buffer buf = allocBuffer(1);

    numero_consumatori_attivi = 1;

    pthread_create(&cons, NULL, &consumatore_bloccante_c1, buf);

    numero_produttori_attivi = 1;

    pthread_create(&prod, NULL, &produttore_bloccante_p1, buf);

    pthread_join(prod, NULL);

    pthread_join(cons, NULL);

    CU_ASSERT_EQUAL(numero_consumatori_attivi, 0);

    CU_ASSERT_EQUAL(numero_produttori_attivi, 0);

    CU_ASSERT_PTR_EQUAL(buf->list_cell_full->buffer_cell_head, NULL);

    freeBuffer(buf);
}

void test_prima_un_produttore_poi_un_consumatore_concorrenti(void) {

    pthread_t prod;
    pthread_t cons;

    buffer buf = allocBuffer(1);

    numero_produttori_attivi = 1;

    pthread_create(&prod, NULL, &produttore_bloccante_p1, buf);

    numero_consumatori_attivi = 1;

    pthread_create(&cons, NULL, &consumatore_bloccante_c1, buf);

    pthread_join(cons, NULL);

    pthread_join(prod, NULL);

    CU_ASSERT_EQUAL(numero_consumatori_attivi, 0);

    CU_ASSERT_EQUAL(numero_produttori_attivi, 0);

    CU_ASSERT_PTR_EQUAL(buf->list_cell_full->buffer_cell_head, NULL);

    freeBuffer(buf);
}

void* produttore_non_bloccante_p2(void *p) {

    void *messaggio = (void *) 3;

    buffer buf = (buffer) p;

    putInBuffer_not_blocking(buf, messaggio);

    numero_produttori_attivi--;

    return NULL;

}

void test_molti_produttori_su_buffer_inizialmente_vuoto(void) {

    pthread_t prod[5];

    int i;

    void *messaggio = (void *) 3;

    buffer buf = allocBuffer(1);

    numero_produttori_attivi = 5;

    for (i = 0; i < 5; i++) {
        pthread_create(&prod[i], NULL, &produttore_non_bloccante_p2, buf);
    }

    for (i = 0; i < 5; i++) {
        pthread_join(prod[i], NULL);
    }

    CU_ASSERT_EQUAL(numero_produttori_attivi, 0);

    CU_ASSERT_EQUAL(buf->list_cell_full->buffer_cell_head->cell_pointer, messaggio);

    freeBuffer(buf);
}

void* consumatore_non_bloccante_c2(void *c) {

    buffer buf = (buffer) c;

    getFromBuffer_not_blocking(buf);

    numero_consumatori_attivi--;

    return NULL;
}

void test_molti_consumatori_su_buffer_inizialmente_pieno(void) {

    pthread_t cons[5];

    int i;

    void *messaggio = (void *) 10;

    buffer buf = allocBuffer(1);

    putInBuffer_blocking(buf, messaggio);

    numero_consumatori_attivi = 5;

    for (i = 0; i < 5; i++) {
        pthread_create(&cons[i], NULL, &consumatore_non_bloccante_c2, buf);
    }
    
    for (i = 0; i < 5; i++) {
        pthread_join(cons[i], NULL);
    }

    CU_ASSERT_EQUAL(numero_consumatori_attivi, 0);

    CU_ASSERT_PTR_EQUAL(buf->list_cell_full->buffer_cell_head, NULL);

    freeBuffer(buf);
}
