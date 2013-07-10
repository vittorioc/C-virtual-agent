#include <stdlib.h>
#include <pthread.h>
#include "list.h"

/*
 * Un oggetto di tipo list è composto da due puntatori, uno che punta alla testa
 * e uno che punta alla coda di una linked-list, e da un mutex per serializzare
 * l'accesso alla lista stessa.
 *
 * L'inserimento di un nuovo elemento avviene in coda mentre l'estrazione
 * avviene in testa
 *
 * Ogni elemento della linked-list è composto da due puntatori, uno che punta
 * all'elemento precedente e uno che punta all'elemento successivo,
 * e dal generico (element) contenuto.
 *
 * Per l'accesso agli elementi della lista viene utilizzato un iteratore,
 * composto da un puntatore alla lista e da un puntatore all'elemento
 * attualmente puntato
 *
 */

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

/*
 * Un mutex è posto a protezione delle operazioni:
 * alloc
 * free
 * lockList
 * unlockList
 *
 */

static pthread_mutex_t list_class_mutex;
static int listObjectsNumber = 0;

static void initClassList() {
    pthread_mutex_init(&(list_class_mutex), NULL);
}

static void cleanClassList() {
    pthread_mutex_destroy(&(list_class_mutex));
}

static int lockList(list _list) {

    int i = 0;

    pthread_mutex_lock(&(list_class_mutex));

    if (_list != NULL) {

        pthread_mutex_lock(&(_list->list_mutex));

        i = 1;
    }

    pthread_mutex_unlock(&(list_class_mutex));

    return i;
}

/*
 * La prima operazione da compiere su una lista è la sua allocazione
 *
 * Per la allocazione della lista non bisogna gestire la concorrenza
 *
 */

list allocList() {

    list list_M_ = NULL;

    if (listObjectsNumber == 0) initClassList();

    listObjectsNumber++;

    list_M_ = (struct list_t *) malloc(sizeof (struct list_t));

    list_M_->list_element_head = NULL;
    list_M_->list_element_tail = NULL;
    pthread_mutex_init(&(list_M_->list_mutex), NULL);

    return list_M_;

}

/*
 * Per la deallocazione della lista (freeList)
 *
 * Corretta sequenza per deallocare una lista per non lasciare puntatori appesi:
 *
 * freeList(lista);
 * lista = NULL;
 *
 */

void freeList(list _F_list) {

    pthread_mutex_lock(&(list_class_mutex));

    if (_F_list != NULL) {

        pthread_mutex_lock(&(_F_list->list_mutex));

        while (_F_list->list_element_head != NULL) {

            list_element _F_list_element = _F_list->list_element_head;
            _F_list->list_element_head = _F_list->list_element_head->list_element_succ;

            free(_F_list_element);
        }

        _F_list->list_element_tail = NULL;
        pthread_mutex_destroy(&(_F_list->list_mutex));

        free(_F_list);
    }

    pthread_mutex_unlock(&(list_class_mutex));

    listObjectsNumber--;

    if (listObjectsNumber == 0) cleanClassList();
}

/* 
 * Aggiunge un elemento in coda
 */

void addElementToList(list _list, element _element) {

    if (lockList(_list)) {

        if (_element != NULL) {

            list_element list_element_M_;

            list_element_M_ = (struct list_element_t *) malloc(sizeof (struct list_element_t));
            list_element_M_->element_pointer = _element;
            list_element_M_->list_element_prev = NULL;
            list_element_M_->list_element_succ = NULL;

            if (_list->list_element_head == NULL) {

                _list->list_element_head = list_element_M_;

            } else {

                list_element_M_->list_element_prev = _list->list_element_tail;

                _list->list_element_tail->list_element_succ = list_element_M_;

            }

            _list->list_element_tail = list_element_M_;

            list_element_M_ = NULL;
        }

        pthread_mutex_unlock(&(_list->list_mutex));
    }
}

/*
 * Crea un iteratore
 * Attenzione: quando viene aggiunto/rimosso un elemento l'iteratore perde
 * di validità e deve essere riallocato
 * Il controllo se l'iteratore è ancora valido deve essere implementato
 */


list_iterator allocListIterator(list _list) {

    list_iterator list_iterator_M_ = NULL;

    if (lockList(_list)) {

        list_iterator_M_ = (struct list_iterator_t *) malloc(sizeof (struct list_iterator_t));
        list_iterator_M_->list_pointer = _list;
        list_iterator_M_->list_element_next = _list->list_element_head;

        pthread_mutex_unlock(&(_list->list_mutex));
    }

    return list_iterator_M_;

}

/* 
 * Dealloca un iteratore
 * Anche qui bisogna stare attenti che tutti i thread abbiano terminato
 * le operazioni con l'iteratore e che un solo thread si stia occupando
 * della sua distruzione
 * Corretta sequenza per deallocare un iteratore senza lasciare puntatori appesi:
 *
 * freeListIterator(iteratore);
 * iteratore = NULL;
 */

void freeListIterator(list_iterator _F_list_iterator) {

    if (_F_list_iterator != NULL) {

        list list_ = _F_list_iterator->list_pointer;

        pthread_mutex_lock(&(list_->list_mutex));

        _F_list_iterator->list_pointer = NULL;
        _F_list_iterator->list_element_next = NULL;
        free(_F_list_iterator);

        pthread_mutex_unlock(&(list_->list_mutex));
    }
}

/*
 * Restituisce 1 se ci sono altri elementi su cui iterare, altrimenti 0
 */

int hasNextList(list_iterator _list_iterator) {

    int i = 0;

    if (_list_iterator != NULL) {

        list list_ = _list_iterator->list_pointer;

        pthread_mutex_lock(&(list_->list_mutex));

        if (_list_iterator->list_element_next != NULL) {
            i = 1;
        }

        pthread_mutex_unlock(&(list_->list_mutex));
    }

    return i;
}

/*
 * Restituisce un nuovo elemento e poi incrementa il puntatore
 * a quello immediatamente successivo
 */

element nextElementFromList(list_iterator _list_iterator) {

    element element_ = NULL;

    if (_list_iterator != NULL) {

        list list_ = _list_iterator->list_pointer;

        pthread_mutex_lock(&(list_->list_mutex));

        if (_list_iterator->list_element_next != NULL) {

            element_ = _list_iterator->list_element_next->element_pointer;

            _list_iterator->list_element_next = _list_iterator->list_element_next->list_element_succ;
        }

        pthread_mutex_unlock(&(list_->list_mutex));
    }

    return element_;
}

/* rimuove ultimo elemento restituito */

void removeLastReturnedElementFromList(list_iterator _list_iterator) {

    if (_list_iterator != NULL) {

        if (_list_iterator->list_pointer->list_element_tail != NULL) {

            /* la lista contiene almeno l'elemento coda */

            if (_list_iterator->list_element_next == NULL) {

                /* next_cell == NULL -> elimino la coda */

                list_element temp = _list_iterator->list_pointer->list_element_tail;

                if (temp->list_element_prev == NULL) {

                    /* la lista contiene un solo elemento e quindi devo annullare anche il campo testa */

                    _list_iterator->list_pointer->list_element_head = NULL;

                    _list_iterator->list_pointer->list_element_tail = NULL;

                } else {

                    _list_iterator->list_pointer->list_element_tail = _list_iterator->list_pointer->list_element_tail->list_element_prev;

                    _list_iterator->list_pointer->list_element_tail->list_element_succ = NULL;

                }

                free(temp);

            } else {

                /* next_cell != NULL -> elimino il precedente a next_cell */

                if (_list_iterator->list_element_next->list_element_prev != NULL) {

                    /* se prev == NULL non ho ancora restituito niente e quindi non devo eliminare niente */

                    list_element temp = _list_iterator->list_element_next->list_element_prev;

                    if (_list_iterator->list_pointer->list_element_head == temp) {

                        _list_iterator->list_pointer->list_element_head = _list_iterator->list_element_next;

                        _list_iterator->list_pointer->list_element_head->list_element_prev = NULL;

                    } else {

                        _list_iterator->list_element_next->list_element_prev = temp->list_element_prev;

                        temp->list_element_prev->list_element_succ = _list_iterator->list_element_next;
                    }

                    free(temp);

                }

            }
        }
    }
}

/*
 * Rimuove un element dalla lista, bisogna implementare la mutua esclusione
 * eliminando le chiamate agli altri metodi della list:
 * allocListIterator
 * hasNextList
 * nextElementFromList
 * removeLastReturnedElementFromList
 */

void removeElementFromList(list _list, element _element) {

    if ((_list != NULL) && (_element != NULL)) {

        list_iterator list_iterator_ = allocListIterator(_list);

        int trovato = 0;

        /*pthread_mutex_lock(&(_list->list_mutex));*/

        while ((hasNextList(list_iterator_) == 1) && (trovato == 0)) {

            if (nextElementFromList(list_iterator_) == _element) {

                trovato = 1;

                removeLastReturnedElementFromList(list_iterator_);
            }
        }

        free(list_iterator_);

        /*pthread_mutex_unlock(&(_list->list_mutex));*/
    }
}
