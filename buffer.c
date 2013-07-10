#include <stdlib.h>
#include <pthread.h>
#include "buffer.h"

/*
 * Un oggetto di tipo buffer è composto da due puntatori, uno che punta alla
 * lista delle celle piene e uno che punta alla lista delle celle vuote, più un
 * mutex per serializzare l'accesso al buffer stesso, più un numero per
 * identificare il buffer
 *
 * Ognuna delle due liste (celle piene/vuote) è composta da un puntatore alla
 * testa della lista e un puntatore alla coda della lista, più un mutex e una
 * variabile condizione per serializzare e sincronizzare gli accessi alla lista
 * L'inserimento in ognuna delle due celle avviene in coda, mentre l'estrazione
 * avviene dalla testa
 *
 * Ognuna delle celle che compongono le due liste (e quindi il buffer)
 * è composta dal contenuto della cella più un puntatore alla cella successiva
 */

struct buffer_cell_t {
    cell cell_pointer;
    struct buffer_cell_t *buffer_cell_succ;
};

struct list_cell_t {
    struct buffer_cell_t *buffer_cell_head;
    struct buffer_cell_t *buffer_cell_tail;
    pthread_mutex_t list_cell_mutex;
    pthread_cond_t list_cell_cond;
};

struct buffer_t {
    int buffer_id;
    struct list_cell_t *list_cell_empty;
    struct list_cell_t *list_cell_full;
    pthread_mutex_t buffer_mutex;
};

typedef struct buffer_cell_t *buffer_cell;
typedef struct list_cell_t *list_cell;

/*
 * Un mutex è posto a protezione delle operazioni:
 * alloc
 * free
 * lockBuffer
 * unlockBuffer
 *
 */

static pthread_mutex_t buffer_class_mutex;
static int bufferObjectsNumber = 0;

static void initClassBuffer() {
    pthread_mutex_init(&(buffer_class_mutex), NULL);
}

static void cleanClassBuffer() {
    pthread_mutex_destroy(&(buffer_class_mutex));
}
/*
static int testExistenceAndLockBuffer(buffer _buffer) {

    int i = 0;

    pthread_mutex_lock(&(buffer_class_mutex));

    if (_buffer != NULL) {

        pthread_mutex_lock(&(_buffer->buffer_mutex));

        i = 1;
    }

    pthread_mutex_unlock(&(buffer_class_mutex));

    return i;
}*/

/*
 * La prima operazione da compiere su un buffer è la sua allocazione,
 * indicando la sua dimensione
 *
 * Per la allocazione del buffer non bisogna gestire la concorrenza
 *
 */

buffer allocBuffer(int dim) {

    buffer buffer_M_;

    list_cell list_cell_empty_M_;
    list_cell list_cell_full_M_;
    buffer_cell buffer_cell1_M_;

    if (bufferObjectsNumber == 0) initClassBuffer();

    bufferObjectsNumber++;

    buffer_M_ = (struct buffer_t *) malloc(sizeof (struct buffer_t));

    list_cell_empty_M_ = (struct list_cell_t *) malloc(sizeof (struct list_cell_t));
    list_cell_full_M_ = (struct list_cell_t *) malloc(sizeof (struct list_cell_t));

    buffer_cell1_M_ = (struct buffer_cell_t *) malloc(sizeof (struct buffer_cell_t));

    buffer_cell1_M_->cell_pointer = NULL;
    buffer_cell1_M_->buffer_cell_succ = NULL;

    list_cell_empty_M_->buffer_cell_head = buffer_cell1_M_;
    list_cell_empty_M_->buffer_cell_tail = buffer_cell1_M_;
    pthread_mutex_init(&(list_cell_empty_M_->list_cell_mutex), NULL);
    pthread_cond_init(&(list_cell_empty_M_->list_cell_cond), NULL);

    list_cell_full_M_->buffer_cell_head = NULL;
    list_cell_full_M_->buffer_cell_tail = NULL;
    pthread_mutex_init(&(list_cell_full_M_->list_cell_mutex), NULL);
    pthread_cond_init(&(list_cell_full_M_->list_cell_cond), NULL);

    buffer_M_->buffer_id = 0;
    buffer_M_->list_cell_empty = list_cell_empty_M_;
    buffer_M_->list_cell_full = list_cell_full_M_;
    pthread_mutex_init(&(buffer_M_->buffer_mutex), NULL);

    if (dim > 1) {

        int v;
        for (v = 1; v < dim; v++) {

            buffer_cell buffer_cell_M_ = (struct buffer_cell_t *) malloc(sizeof (struct buffer_cell_t));

            buffer_cell_M_->cell_pointer = NULL;
            buffer_cell_M_->buffer_cell_succ = NULL;

            buffer_M_->list_cell_empty->buffer_cell_tail->buffer_cell_succ = buffer_cell_M_;
            buffer_M_->list_cell_empty->buffer_cell_tail = buffer_cell_M_;
        }
    }

    return buffer_M_;
}

/*
 * Per la deallocazione del buffer (freeBuffer)
 *
 * Corretta sequenza per deallocare un buffer per non lasciare puntatori appesi:
 *
 * freeBuffer(buffer);
 * buffer = NULL;
 *
 */

void freeBuffer(buffer _F_buffer) {

    pthread_mutex_lock(&(buffer_class_mutex));

    if (_F_buffer != NULL) {

        pthread_mutex_lock(&(_F_buffer->buffer_mutex));

        while (_F_buffer->list_cell_empty->buffer_cell_head != NULL) {

            buffer_cell temp = _F_buffer->list_cell_empty->buffer_cell_head;
            _F_buffer->list_cell_empty->buffer_cell_head = _F_buffer->list_cell_empty->buffer_cell_head->buffer_cell_succ;

            temp->buffer_cell_succ = NULL;
            free(temp);
            temp = NULL;
        }
        _F_buffer->list_cell_empty->buffer_cell_tail = NULL;

        while (_F_buffer->list_cell_full->buffer_cell_head != NULL) {

            buffer_cell temp = _F_buffer->list_cell_full->buffer_cell_head;
            _F_buffer->list_cell_full->buffer_cell_head = _F_buffer->list_cell_full->buffer_cell_head->buffer_cell_succ;

            temp->buffer_cell_succ = NULL;
            free(temp);
            temp = NULL;
        }
        _F_buffer->list_cell_full->buffer_cell_tail = NULL;

        free(_F_buffer->list_cell_empty);
        _F_buffer->list_cell_empty = NULL;
        free(_F_buffer->list_cell_full);
        _F_buffer->list_cell_full = NULL;

        free(_F_buffer);
    }
    pthread_mutex_unlock(&(buffer_class_mutex));

    bufferObjectsNumber--;

    if (bufferObjectsNumber == 0) cleanClassBuffer();

}

void setBufferID(buffer _buffer, int _id) {

    _buffer->buffer_id = _id;

}

int getBufferID(buffer _buffer) {

    return _buffer->buffer_id;

}

/*
 * inserimento bloccante: sospende se pieno, quindi
 * effettua l’inserimento non appena si libera dello spazio
 */

void putInBuffer_blocking(buffer _buffer, cell _cell) {

    buffer_cell temp_cell;

    pthread_mutex_lock(&(_buffer->list_cell_empty->list_cell_mutex));

    /*
     * quando ottengo il mutex alla lista delle celle vuote controllo che ce ne siano di disponibili
     * se non ce ne sono mi metto in attesa di una nuova consumazione e rilascio il mutex alla lista
     */

    while (_buffer->list_cell_empty->buffer_cell_head == NULL) {
        pthread_cond_wait(&(_buffer->list_cell_empty->list_cell_cond), &(_buffer->list_cell_empty->list_cell_mutex));
    }

    /*
     * estrazione di una cella dalla lista delle celle vuote
     */

    /*
     * uso un puntatore di appoggio per memorizzare la cella che andrò ad estrarre dalla lista delle celle vuote
     */

    temp_cell = _buffer->list_cell_empty->buffer_cell_head;

    /*
     * se la lista delle celle vuote contiene solo la cella che sto per estrarre
     * allora quando la avrò estratta non ci saranno più celle disponibili
     * quindi imposto il puntatore alla coda della lista a NULL
     */

    if (_buffer->list_cell_empty->buffer_cell_head->buffer_cell_succ == NULL) {
        _buffer->list_cell_empty->buffer_cell_tail = NULL;
    }

    /*
     * estraggo la cella dalla lista: comunque ho memorizzato il suo puntatore in temp_cell
     */

    _buffer->list_cell_empty->buffer_cell_head = _buffer->list_cell_empty->buffer_cell_head->buffer_cell_succ;

    /*
     * ho finito il lavoro con la lista delle celle vuote e quindi rilascio il mutex
     */

    pthread_mutex_unlock(&(_buffer->list_cell_empty->list_cell_mutex));

    /*
     * inserisco il messaggio nella cella e pulisco il suo campo next
     */

    temp_cell->cell_pointer = _cell;
    temp_cell->buffer_cell_succ = NULL;

    pthread_mutex_lock(&(_buffer->list_cell_full->list_cell_mutex));

    /*
     * quando ottengo il mutex alla lista delle celle piene inserisco la nuova cella in coda
     */

    if (_buffer->list_cell_full->buffer_cell_tail == NULL) {
        _buffer->list_cell_full->buffer_cell_head = temp_cell;
    } else {
        _buffer->list_cell_full->buffer_cell_tail->buffer_cell_succ = temp_cell;
    }

    _buffer->list_cell_full->buffer_cell_tail = temp_cell;

    temp_cell = NULL;

    /*
     * segnalo che è avvenuta una nuova produzione e rilascio il mutex alla lista delle celle piene
     */

    pthread_cond_signal(&(_buffer->list_cell_full->list_cell_cond));

    pthread_mutex_unlock(&(_buffer->list_cell_full->list_cell_mutex));

}

/*
 *  inserimento non bloccante: restituisce BUFFER_PIENO se pieno,
 *  altrimenti restituisce BUFFER_OK ed effettua l’inserimento
 */

int putInBuffer_not_blocking(buffer _buffer, cell _cell) {

    pthread_mutex_lock(&(_buffer->list_cell_empty->list_cell_mutex));

    /*
     * quando ottengo il mutex alla lista delle celle vuote controllo che ce ne siano di disponibili
     */

    if (_buffer->list_cell_empty->buffer_cell_head == NULL) {

        /*
         * se non ce ne sono rilascio il mutex alla lista ed esco ritornando il valore BUFFER_PIENO
         */

        pthread_mutex_unlock(&(_buffer->list_cell_empty->list_cell_mutex));

        return BUFFER_PIENO;

    } else {

        /*
         * altrimenti vuol dire che c'è almeno una cella disponibile e quindi procedo esattamente come ho fatto con la put_bloccante
         */

        buffer_cell temp_cell = _buffer->list_cell_empty->buffer_cell_head;

        if (_buffer->list_cell_empty->buffer_cell_head->buffer_cell_succ == NULL) {
            _buffer->list_cell_empty->buffer_cell_tail = NULL;
        }

        _buffer->list_cell_empty->buffer_cell_head = _buffer->list_cell_empty->buffer_cell_head->buffer_cell_succ;


        pthread_mutex_unlock(&(_buffer->list_cell_empty->list_cell_mutex));

        temp_cell->cell_pointer = _cell;
        temp_cell->buffer_cell_succ = NULL;

        pthread_mutex_lock(&(_buffer->list_cell_full->list_cell_mutex));

        if (_buffer->list_cell_full->buffer_cell_tail == NULL) {
            _buffer->list_cell_full->buffer_cell_head = temp_cell;
        } else {
            _buffer->list_cell_full->buffer_cell_tail->buffer_cell_succ = temp_cell;
        }

        _buffer->list_cell_full->buffer_cell_tail = temp_cell;

        temp_cell = NULL;

        pthread_cond_signal(&(_buffer->list_cell_full->list_cell_cond));

        pthread_mutex_unlock(&(_buffer->list_cell_full->list_cell_mutex));

        return BUFFER_OK;
    }
}

/*
 *  estrazione bloccante: sospende se vuoto, quindi
 *  restituisce il valore estratto non appena disponibile
 */

cell getFromBuffer_blocking(buffer _buffer) {

    void *get_msg;

    buffer_cell temp_cell;

    pthread_mutex_lock(&(_buffer->list_cell_full->list_cell_mutex));

    /*
     * quando ottengo il mutex alla lista delle celle piene controllo che ce ne siano di disponibili
     * se non ce ne sono mi metto in attesa di una nuova produzione e rilascio il mutex alla lista
     */

    while (_buffer->list_cell_full->buffer_cell_head == NULL) {
        pthread_cond_wait(&(_buffer->list_cell_full->list_cell_cond), &(_buffer->list_cell_full->list_cell_mutex));
    };

    /*
     * estrazione di una cella dalla lista delle celle piene
     */

    /*
     * uso un puntatore di appoggio per memorizzare la cella che andrò ad estrarre dalla lista delle celle piene
     */

    temp_cell = _buffer->list_cell_full->buffer_cell_head;

    /*
     * se la lista delle celle piene contiene solo la cella che sto per estrarre
     * allora quando la avrò estratta non ci saranno più celle disponibili
     * quindi imposto il puntatore alla coda della lista a NULL
     */

    if (_buffer->list_cell_full->buffer_cell_head->buffer_cell_succ == NULL) {
        _buffer->list_cell_full->buffer_cell_tail = NULL;
    }

    /*
     * estraggo la cella dalla lista: comunque ho memorizzato il suo puntatore in temp_cell
     */

    _buffer->list_cell_full->buffer_cell_head = _buffer->list_cell_full->buffer_cell_head->buffer_cell_succ;

    /*
     * ho finito il lavoro con la lista delle celle piene e quindi rilascio il mutex
     */

    pthread_mutex_unlock(&(_buffer->list_cell_full->list_cell_mutex));

    /*
     * estraggo il messaggio nella cella e lo memorizzo in get_msg
     * successivamente azzero il contenuto della cella e pulisco il suo campo next
     */

    get_msg = temp_cell->cell_pointer;
    temp_cell->cell_pointer = NULL;
    temp_cell->buffer_cell_succ = NULL;

    pthread_mutex_lock(&(_buffer->list_cell_empty->list_cell_mutex));

    /*
     * quando ottengo il mutex alla lista delle celle vuote inserisco la nuova cella in coda
     */

    if (_buffer->list_cell_empty->buffer_cell_tail == NULL) {
        _buffer->list_cell_empty->buffer_cell_head = temp_cell;
    } else {
        _buffer->list_cell_empty->buffer_cell_tail->buffer_cell_succ = temp_cell;
    }

    _buffer->list_cell_empty->buffer_cell_tail = temp_cell;

    temp_cell = NULL;

    /*
     * segnalo che è avvenuta una nuova consumazione e rilascio il mutex alla lista delle celle vuote
     */

    pthread_cond_signal(&(_buffer->list_cell_empty->list_cell_cond));

    pthread_mutex_unlock(&(_buffer->list_cell_empty->list_cell_mutex));

    /*
     * ritorno il messaggio estratto dalla cella
     */

    return get_msg;

}

/*
 *  estrazione non bloccante: restituisce BUFFER_VUOTO se vuoto
 *  ed il valore estratto in caso contrario
 */

cell getFromBuffer_not_blocking(buffer _buffer) {

    void *get_msg;

    pthread_mutex_lock(&(_buffer->list_cell_full->list_cell_mutex));

    /*
     * quando ottengo il mutex alla lista delle celle piene controllo che ce ne siano di disponibili
     */

    if (_buffer->list_cell_full->buffer_cell_head == NULL) {

        /*
         * se non ce ne sono rilascio il mutex alla lista ed esco ritornando il valore BUFFER_VUOTO
         */

        pthread_mutex_unlock(&(_buffer->list_cell_full->list_cell_mutex));

        return (void *) BUFFER_VUOTO;

    } else {

        /*
         * altrimenti vuol dire che c'è almeno una cella disponibile e quindi procedo esattamente come ho fatto con la get_bloccante
         */

        buffer_cell temp_cell;

        temp_cell = _buffer->list_cell_full->buffer_cell_head;

        if (_buffer->list_cell_full->buffer_cell_head->buffer_cell_succ == NULL) {
            _buffer->list_cell_full->buffer_cell_tail = NULL;
        }

        _buffer->list_cell_full->buffer_cell_head = _buffer->list_cell_full->buffer_cell_head->buffer_cell_succ;

        pthread_mutex_unlock(&(_buffer->list_cell_full->list_cell_mutex));

        get_msg = temp_cell->cell_pointer;
        temp_cell->cell_pointer = NULL;
        temp_cell->buffer_cell_succ = NULL;

        pthread_mutex_lock(&(_buffer->list_cell_empty->list_cell_mutex));

        if (_buffer->list_cell_empty->buffer_cell_tail == NULL) {
            _buffer->list_cell_empty->buffer_cell_head = temp_cell;
        } else {
            _buffer->list_cell_empty->buffer_cell_tail->buffer_cell_succ = temp_cell;
        }

        _buffer->list_cell_empty->buffer_cell_tail = temp_cell;

        temp_cell = NULL;

        pthread_cond_signal(&(_buffer->list_cell_empty->list_cell_cond));

        pthread_mutex_unlock(&(_buffer->list_cell_empty->list_cell_mutex));

        return get_msg;
    }
}

void emptyBuffer(buffer _buffer) {

    _buffer = NULL;
    /*
    while (getFromBuffer_not_blocking(_buffer) != BUFFER_VUOTO) {
    }
     */
}

int isEmptyBuffer(buffer _buffer) {

    int i = 0;

    if (_buffer->list_cell_full->buffer_cell_head == NULL) {
        i = 1;
    };

    return i;
}
