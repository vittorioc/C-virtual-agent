#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/* inet_aton -> per utilizzarla in gcc: __USE_MISC */
#define __USE_MISC
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "buffer.h"
#include "list.h"
#include "router.h"
#include <gsl/gsl_matrix.h>

#define DIM_BUFFER 1000
#define POISON_ID -1

#define AGENT_NUMBER 2

struct msg_t {
    int msg_id;
    int msg_time;
    float msg_x;
    float msg_z;
};

struct routing_table_t {
    int dimension;
    list list_table;
};

struct router_t {
    int router_id;
    struct routing_table_t routing_table;
    pthread_mutex_t router_mutex;
};

struct parametri_accepter_t {
    int accepter_agents_number;
    char *accepter_address;
    int accepter_port;
    list accepter_lista_buffer;
    int accepter_in_out;
};

struct parametri_dispatcher_t {
    buffer dispatcher_buffer_in;
    list dispatcher_lista_buffer_out;
};

struct parametri_receiver_t {
    int receiver_id;
    int receiver_socket;
    buffer receiver_buffer;
};

struct parametri_sender_t {
    int sender_id;
    int sender_socket;
    buffer sender_buffer;
};

typedef struct msg_t *msg;

typedef struct parametri_accepter_t *parametri_accepter;
typedef struct parametri_dispatcher_t *parametri_dispatcher;
typedef struct parametri_receiver_t *parametri_receiver;
typedef struct parametri_sender_t *parametri_sender;

static pthread_mutex_t router_class_mutex;
static int routerObjectsNumber = 0;

static int numero_receiver_vivi = 0;
static int numero_dispatcher_vivi = 0;
static int numero_sender_vivi = 0;

static void initClassRouter() {
    pthread_mutex_init(&(router_class_mutex), NULL);
}

static void cleanClassRouter() {
    pthread_mutex_destroy(&(router_class_mutex));

}

router allocRouter() {

    router router_M_;

    if (routerObjectsNumber == 0) initClassRouter();

    routerObjectsNumber++;

    router_M_ = (router) malloc(sizeof (struct router_t));

    pthread_mutex_init(&(router_M_->router_mutex), NULL);

    return router_M_;

}

void freeRouter(router _F_router) {

    pthread_mutex_destroy(&(_F_router->router_mutex));

    free(_F_router);

    _F_router = NULL;

    routerObjectsNumber--;

    if (routerObjectsNumber == 0) cleanClassRouter();
}

static int listenFrom(char * _ip_address_local, int _ip_port_local) {

    int socket_M_;

    struct sockaddr_in sockaddr_in_local_;

    memset(&sockaddr_in_local_, 0, sizeof (sockaddr_in_local_));
    sockaddr_in_local_.sin_family = AF_INET;
    sockaddr_in_local_.sin_port = htons(_ip_port_local);
    inet_aton(_ip_address_local, &(sockaddr_in_local_.sin_addr));

    if ((socket_M_ = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("listenFrom: socket() Error\n");
        exit(1);
    }

    if (bind(socket_M_, (struct sockaddr *) & sockaddr_in_local_, sizeof (struct sockaddr_in)) == -1) {
        perror("listenFrom: bind() Error\n");
        exit(1);
    }

    if (listen(socket_M_, 1) == -1) {
        perror("listenFrom: listen() Error\n");
        exit(1);
    }

    return socket_M_;

}

static void sendMsg(int _socket, msg _msg) {

    if (send(_socket, _msg, sizeof (struct msg_t), 0) == -1) {
        perror("sendMsg: send() Error\n");
        exit(1);
    };
    /*
        int net_id;
        int net_x;
        int net_z;

        net_id = htonl(_msg->msg_id);
        if (send(_socket, &net_id, sizeof (net_id), 0) == -1) {
            perror("sendMsg: send() Error\n");
            exit(1);
        };

        net_x = htonl(_msg->msg_x);
        if (send(_socket, &net_x, sizeof (net_x), 0) == -1) {
            perror("sendMsg: send() Error\n");
            exit(1);
        };

        net_z = htonl(_msg->msg_z);
        if (send(_socket, &net_z, sizeof (net_z), 0) == -1) {
            perror("sendMsg: send() Error\n");
            exit(1);
        };
     */
}

static msg recvMsg(int _socket) {

    msg _M_msg = (struct msg_t *) malloc(sizeof (struct msg_t));

    if (recv(_socket, _M_msg, sizeof (struct msg_t), 0) == -1) {
        perror("recvMsg: recv() Error\n");
        exit(1);
    }
    /*
        int net_id;
        int net_x;
        int net_z;

        msg _M_msg = (struct msg_t *) malloc(sizeof (struct msg_t));

        if (recv(_socket, &net_id, sizeof (net_id), 0) == -1) {
            perror("recvMsg: recv() Error\n");
            exit(1);
        }
        _M_msg->msg_id = ntohl(net_id);

        if (recv(_socket, &net_x, sizeof (net_x), 0) == -1) {
            perror("recvMsg: recv() Error\n");
            exit(1);
        }
        _M_msg->msg_x = ntohl(net_x);

        if (recv(_socket, &net_z, sizeof (net_z), 0) == -1) {
            perror("recvMsg: recv() Error\n");
            exit(1);
        }
        _M_msg->msg_z = ntohl(net_z);
     */
    return _M_msg;
}

static parametri_receiver allocParamReceiver(int _receiver_socket, buffer _receiver_buffer) {

    parametri_receiver _M_parametri_receiver = (parametri_receiver) malloc(sizeof (struct parametri_receiver_t));

    _M_parametri_receiver->receiver_socket = _receiver_socket;
    _M_parametri_receiver->receiver_buffer = _receiver_buffer;

    return _M_parametri_receiver;
}

/*
 * Quando il Receiver riceve dall'agent un messaggio con id = POISON_ID allora:
 * - lo inoltra nel buffer
 * - chiude la connessione con l'agent
 * - fa il fflush del file di log e lo chiude
 * - decrementa numero_receiver_vivi
 * - termina
 */

static void *runReceiver(void *_parametri_receiver) {

    parametri_receiver _parametri = (parametri_receiver) _parametri_receiver;

    int _receiver_socket = _parametri->receiver_socket;
    buffer _receiver_buffer = _parametri->receiver_buffer;

    msg _msg;
    int _receiver_id;

    FILE *receiver_log;

    char receiver_log_file_name[FILENAME_MAX];

    /*
     * Riceve il messaggio di presentazione dall'agent
     * - imposta l'id del receiver
     * - imposta l'id del _receiver_buffer
     * - reinvia all'agent il messaggio ricevuto
     */

    _msg = recvMsg(_receiver_socket);
    _receiver_id = _msg->msg_id;
    setBufferID(_receiver_buffer, _receiver_id);

    /* 
     * Apre il file di log
     */

    sprintf(receiver_log_file_name, "receiver_%d.log", _receiver_id);

    receiver_log = fopen(receiver_log_file_name, "w");

    if (receiver_log == NULL) {
        perror("Errore apertura file: \"receiver.log\"\n");
        exit(1);
    };

    setbuf(receiver_log, NULL);

    fprintf(receiver_log, "R%d: Open log\n", _receiver_id);

    /*sleep(1);*/

    fprintf(receiver_log, "R%d: Parametri invocazione:\n", _receiver_id);

    fprintf(receiver_log, "R%d: Wait: rcv\n", _receiver_id);

    fprintf(receiver_log, "R%d: > (%d,%d,%f,%f)\n", _receiver_id, _msg->msg_id, _msg->msg_time, _msg->msg_x, _msg->msg_z);

    fprintf(receiver_log, "R%d: Wait: rcv\n", _receiver_id);

    /* 
     * Riceve un messaggio e lo pone nel buffer finchè non riceve il POISON_ID
     */

    _msg = recvMsg(_receiver_socket);

    fprintf(receiver_log, "R%d: > (%d,%d,%f,%f)\n", _receiver_id, _msg->msg_id, _msg->msg_time, _msg->msg_x, _msg->msg_z);

    fprintf(receiver_log, "R%d: Loop:\n", _receiver_id);

    fprintf(receiver_log, "R%d: > while(msg != POISON_ID)\n", _receiver_id);

    fprintf(receiver_log, "R%d: --- Loop Start ---\n", _receiver_id);

    while (_msg->msg_id != POISON_ID) {

        putInBuffer_blocking(_receiver_buffer, _msg);

        fprintf(receiver_log, "R%d: >>> putInBuffer(%d,%d,%f,%f)\n", _receiver_id, _msg->msg_id, _msg->msg_time, _msg->msg_x, _msg->msg_z);

        fprintf(receiver_log, "R%d: >>> Wait: rcv\n", _receiver_id);

        _msg = recvMsg(_receiver_socket);
    };

    fprintf(receiver_log, "R%d: ---- Loop End ----\n", _receiver_id);

    fprintf(receiver_log, "R%d: Signal:\n", _receiver_id);

    putInBuffer_blocking(_receiver_buffer, _msg);

    fprintf(receiver_log, "R%d: > putInBuffer(%d,%d,%f,%f) \n", _receiver_id, _msg->msg_id, _msg->msg_time, _msg->msg_x, _msg->msg_z);

    fprintf(receiver_log, "R%d: Socket:\n", _receiver_id);

    /* 
     * Chiude la connessione con l'agent
     */

    send(_receiver_socket, "", 1, 0);

    close(_receiver_socket);

    fprintf(receiver_log, "R%d: > close receiver socket\n", _receiver_id);

    fprintf(receiver_log, "R%d: Memoria\n", _receiver_id);

    free(_parametri_receiver);

    fprintf(receiver_log, "R%d: > parametri\n", _receiver_id);

    /*
     * fflush e chiusura del logfile
     */

    fprintf(receiver_log, "R%d: Close log\n", _receiver_id);

    fflush(receiver_log);

    fclose(receiver_log);

    /* Decrementa numero_receiver_vivi e termina*/

    numero_receiver_vivi--;

    return NULL;
}

static parametri_sender allocParamSender(int _sender_socket, buffer _sender_buffer) {

    parametri_sender _M_parametri_sender = (parametri_sender) malloc(sizeof (struct parametri_sender_t));

    _M_parametri_sender->sender_socket = _sender_socket;
    _M_parametri_sender->sender_buffer = _sender_buffer;

    return _M_parametri_sender;
}

/*
 * Quando il Sender prende dal buffer un messaggio con id = POISON_ID
 * vuol dire che quello è l'ultimo messaggio e il buffer non ne contiene altri:
 * - attende che i dispatcher siano morti
 * - elimina il sender buffer associato
 * - chiude la connessione con l'agent
 * - fa il fflush del file di log e lo chiude
 * - decrementa numero_sender_buffer
 * - termina
 */

static void *runSender(void *_parametri_sender) {

    parametri_sender _parametri = (parametri_sender) _parametri_sender;

    int _sender_socket = _parametri->sender_socket;
    buffer _sender_buffer = _parametri->sender_buffer;

    int _sender_id;

    msg _msg;

    FILE *sender_log;

    char sender_log_file_name[FILENAME_MAX];

    /*
     * Riceve il messaggio di presentazione dall'agent:
     * - imposta l'id del sender
     * - imposta l'id del _sender_buffer
     * - reinvia all'agent il messaggio ricevuto
     */

    _msg = recvMsg(_sender_socket);
    _sender_id = _msg->msg_id;
    setBufferID(_sender_buffer, _sender_id);

    /* 
     * Apre il file di log
     */

    sprintf(sender_log_file_name, "sender_%d.log", _sender_id);

    sender_log = fopen(sender_log_file_name, "w");

    if (sender_log == NULL) {
        perror("Errore apertura file: \"sender.log\"\n");
        exit(1);
    };

    setbuf(sender_log, NULL);

    fprintf(sender_log, "S%d: Open log\n", _sender_id);

    /*sleep(1);*/

    fprintf(sender_log, "S%d: Parametri di invocazione:\n", _sender_id);

    fprintf(sender_log, "S%d: Wait: rcv\n", _sender_id);

    fprintf(sender_log, "S%d: > (%d,%d,%f,%f)\n", _sender_id, _msg->msg_id, _msg->msg_time, _msg->msg_x, _msg->msg_z);

    fprintf(sender_log, "S%d: Wait: getFromBuffer\n", _sender_id);

    /* 
     * Riceve un messaggio e lo pone nel buffer finchè non riceve il POISON_ID
     */

    _msg = getFromBuffer_blocking(_sender_buffer);

    fprintf(sender_log, "S%d: > (%d,%d,%f,%f)\n", _sender_id, _msg->msg_id, _msg->msg_time, _msg->msg_x, _msg->msg_z);

    fprintf(sender_log, "S%d: Loop:\n", _sender_id);

    fprintf(sender_log, "S%d: > while(msg != POISON_ID)\n", _sender_id);

    fprintf(sender_log, "S%d: --- Loop Start ---\n", _sender_id);

    while (_msg->msg_id != POISON_ID) {

        sendMsg(_sender_socket, _msg);

        fprintf(sender_log, "S%d: >>> snd (%d,%d,%f,%f)\n", _sender_id, _msg->msg_id, _msg->msg_time, _msg->msg_x, _msg->msg_z);

        fprintf(sender_log, "S%d: >>> Wait: getFromBuffer\n", _sender_id);

        _msg = getFromBuffer_blocking(_sender_buffer);
    };

    fprintf(sender_log, "S%d: ---- Loop End ----\n", _sender_id);

    fprintf(sender_log, "S%d: Wait:\n", _sender_id);

    /* 
     * Attende che tutti i dispatcher siano morti
     */

    fprintf(sender_log, "S%d: > while(dispatcher > 0)\n", _sender_id);

    while (numero_dispatcher_vivi > 0);

    fprintf(sender_log, "S%d: Socket:\n", _sender_id);

    /*
     * Chiude la connessione con l'agent
     */

    send(_sender_socket, "", 1, 0);

    close(_sender_socket);

    fprintf(sender_log, "S%d: > close sender socket\n", _sender_id);

    fprintf(sender_log, "S%d: Memoria:\n", _sender_id);

    /* 
     * Elimina il receiver buffer associato
     */

    freeBuffer(_sender_buffer);

    fprintf(sender_log, "S%d: > free(sender_buffer)\n", _sender_id);

    free(_parametri_sender);

    fprintf(sender_log, "S%d: > parametri\n", _sender_id);

    /*
     * fflush e chiusura del logfile
     */

    fprintf(sender_log, "S%d: Close log\n", _sender_id);

    fflush(sender_log);

    fclose(sender_log);

    /* Decrementa numero_sender_vivi e termina*/

    numero_sender_vivi--;

    return NULL;
}

static parametri_accepter allocParamAccepter(int _accepter_agents_number, char *_accepter_address, int _accepter_port, list _accepter_lista_buffer, int _accepter_in_out) {

    parametri_accepter _M_parametri_accepter = (parametri_accepter) malloc(sizeof (struct parametri_accepter_t));

    _M_parametri_accepter->accepter_agents_number = _accepter_agents_number;
    _M_parametri_accepter->accepter_address = _accepter_address;
    _M_parametri_accepter->accepter_port = _accepter_port;
    _M_parametri_accepter->accepter_lista_buffer = _accepter_lista_buffer;
    _M_parametri_accepter->accepter_in_out = _accepter_in_out;

    return _M_parametri_accepter;

}

/*
 * Quando l'accepter riceve _accepter_agents_number connessioni in input e in output
 * vuol dire che tutti gli agents si sono collegati:
 * - chiude le due porte in listening
 * - fa il fflush del file di log e lo chiude
 * - termina
 */

static void *runAccepter(void *_parametri_accepter) {

    parametri_accepter _parametri = (parametri_accepter) _parametri_accepter;

    int _accepter_agents_number;
    char *_accepter_address;
    int _accepter_port;
    list _accepter_lista_buffer;
    int _accepter_in_out;

    int listening_socket;
    int communication_socket;

    FILE *accepter_log;

    char *accepter_log_file_name;
    char *in_out;
    char *accepter_error;

    /* Apre il file di log */

    _accepter_in_out = _parametri->accepter_in_out;

    if (_accepter_in_out == 0) {
        accepter_log_file_name = "accepter_in.log";
        accepter_error = "Errore apertura file: \"accepter_in.log\"\n";
        in_out = "AI";
    } else {
        accepter_log_file_name = "accepter_out.log";
        accepter_error = "Errore apertura file: \"accepter_out.log\"\n";
        in_out = "AO";
    };

    accepter_log = fopen(accepter_log_file_name, "w");

    if (accepter_log == NULL) {
        perror(accepter_error);
        exit(1);
    };

    setbuf(accepter_log, NULL);

    fprintf(accepter_log, "%s: Open log\n", in_out);

    fprintf(accepter_log, "%s: Parametri di invocazione:\n", in_out);

    _accepter_agents_number = _parametri->accepter_agents_number;

    fprintf(accepter_log, "%s: > %d\n", in_out, _accepter_agents_number);

    _accepter_address = _parametri->accepter_address;

    fprintf(accepter_log, "%s: > %s\n", in_out, _accepter_address);

    _accepter_port = _parametri->accepter_port;

    fprintf(accepter_log, "%s: > %d\n", in_out, _accepter_port);

    _accepter_lista_buffer = _parametri->accepter_lista_buffer;

    fprintf(accepter_log, "%s: > %d\n", in_out, _accepter_in_out);

    fprintf(accepter_log, "%s: Socket:\n", in_out);

    fprintf(accepter_log, "%s: > listenFrom(%s,%d)\n", in_out, _accepter_address, _accepter_port);

    listening_socket = listenFrom(_accepter_address, _accepter_port);

    if (_accepter_in_out == 0) {

        fprintf(accepter_log, "%s: Loop:\n", in_out);

        fprintf(accepter_log, "%s: > while(receiver < agents)\n", in_out);

        fprintf(accepter_log, "%s: ---------- Loop Start ----------\n", in_out);

        while (numero_receiver_vivi < _accepter_agents_number) {

            buffer _M_buffer = allocBuffer(DIM_BUFFER);

            pthread_t receiver_;

            if ((communication_socket = accept(listening_socket, NULL, NULL)) == -1) {
                perror("Accepter accept() Error\n");
                exit(1);
            }

            fprintf(accepter_log, "%s: >>> accept connection\n", in_out);

            addElementToList(_accepter_lista_buffer, _M_buffer);

            numero_receiver_vivi++;

            pthread_create(&receiver_, NULL, &runReceiver, allocParamReceiver(communication_socket, _M_buffer));
        }
        fprintf(accepter_log, "%s: ----------- Loop End -----------\n", in_out);


    } else {

        fprintf(accepter_log, "%s: Loop:\n", in_out);

        fprintf(accepter_log, "%s: > while(sender < agents)\n", in_out);

        fprintf(accepter_log, "%s: --------- Loop Start ---------\n", in_out);

        while (numero_sender_vivi < _accepter_agents_number) {

            buffer _M_buffer = allocBuffer(DIM_BUFFER);

            pthread_t sender_;

            if ((communication_socket = accept(listening_socket, NULL, NULL)) == -1) {
                perror("Accepter accept() Error\n");
                exit(1);
            }

            fprintf(accepter_log, "%s: >>> accept connection\n", in_out);

            addElementToList(_accepter_lista_buffer, _M_buffer);

            numero_sender_vivi++;

            pthread_create(&sender_, NULL, &runSender, allocParamSender(communication_socket, _M_buffer));
        }

        fprintf(accepter_log, "%s: ---------- Loop End ----------\n", in_out);

    }

    fprintf(accepter_log, "%s: Socket:\n", in_out);

    /* 
     * Chiude le due porte in listening
     */

    close(listening_socket);

    fprintf(accepter_log, "%s: > close listen socket\n", in_out);

    fprintf(accepter_log, "%s: Memoria:\n", in_out);

    free(_parametri_accepter);

    fprintf(accepter_log, "%s: > parametri\n", in_out);

    /*
     * fflush e chiusura del logfile
     */

    fprintf(accepter_log, "%s: Close log\n", in_out);

    fflush(accepter_log);

    fclose(accepter_log);

    return NULL;

}

static parametri_dispatcher allocParamDispatcher(buffer _dispatcher_buffer_in, list _dispatcher_lista_buffer_out) {

    parametri_dispatcher _M_parametri_dispatcher = (parametri_dispatcher) malloc(sizeof (struct parametri_dispatcher_t));

    _M_parametri_dispatcher->dispatcher_buffer_in = _dispatcher_buffer_in;
    _M_parametri_dispatcher->dispatcher_lista_buffer_out = _dispatcher_lista_buffer_out;

    return _M_parametri_dispatcher;

}

/*
 * Quando il dispatcher riceve il POISON_ID allora:
 * - lo inoltra nel sender buffer associato e solo in quello
 * - attende che tutti i receiver siano morti
 * - elimina il receiver buffer associato
 * - fa il fflush del file di log e lo chiude
 * - decrementa numero_dispatcher_vivi
 * - termina
 */

static void *runDispatcher(void *_parametri_dispatcher) {

    parametri_dispatcher _parametri = (parametri_dispatcher) _parametri_dispatcher;

    buffer _dispatcher_buffer_in = _parametri->dispatcher_buffer_in;
    list _dispatcher_lista_buffer_out;

    int _dispatcher_id;
    int _sender_buffer_id;

    list_iterator it_out;
    buffer _sender_buffer;

    msg _msg;

    gsl_matrix *_dispatcher_adjacency_matrix;

    FILE *dispatcher_adjacency_matrix_file;

    FILE *dispatcher_log;

    char dispatcher_log_file_name[FILENAME_MAX];

    _dispatcher_adjacency_matrix = gsl_matrix_alloc(AGENT_NUMBER, AGENT_NUMBER);
    dispatcher_adjacency_matrix_file = fopen("adjacency_matrix.txt", "r");
    gsl_matrix_fscanf(dispatcher_adjacency_matrix_file, _dispatcher_adjacency_matrix);
    fclose(dispatcher_adjacency_matrix_file);

    /*
    gsl_matrix_fprintf(stdout, adjacency_matrix, "%f");
    printf("valore 0,0 = %f\n",gsl_matrix_get(adjacency_matrix,0,0));
    printf("valore 1,0 = %f\n",gsl_matrix_get(adjacency_matrix,1,0));
    printf("valore 0,1 = %f\n",gsl_matrix_get(adjacency_matrix,0,1));
    printf("valore 1,1 = %f\n",gsl_matrix_get(adjacency_matrix,1,1));
     */

    /* Apre il file di log impostando il suo id a quello del buffer associato */

    _dispatcher_id = getBufferID(_dispatcher_buffer_in);

    sprintf(dispatcher_log_file_name, "dispatcher_%d.log", _dispatcher_id);

    dispatcher_log = fopen(dispatcher_log_file_name, "w");

    if (dispatcher_log == NULL) {
        perror("Errore apertura file: \"dispatcher.log\"\n");
        exit(1);
    };

    setbuf(dispatcher_log, NULL);

    fprintf(dispatcher_log, "D%d: Open log\n", _dispatcher_id);

    /*sleep(1);*/

    fprintf(dispatcher_log, "D%d: Parametri di invocazione:\n", _dispatcher_id);

    _dispatcher_lista_buffer_out = _parametri->dispatcher_lista_buffer_out;

    fprintf(dispatcher_log, "D%d: Wait:\n", _dispatcher_id);

    fprintf(dispatcher_log, "D%d: > getFromBuffer(receiver_buffer)\n", _dispatcher_id);

    /*
     * Riceve un messaggio e lo pone in tutti i sender buffer finchè non riceve il POISON_ID
     */

    _msg = getFromBuffer_blocking(_dispatcher_buffer_in);

    fprintf(dispatcher_log, "D%d: ----------- Loop Start -----------\n", _dispatcher_id);

    while (_msg->msg_id != POISON_ID) {

        it_out = allocListIterator(_dispatcher_lista_buffer_out);

        while (hasNextList(it_out) == 1) {

            int i;

            _sender_buffer = nextElementFromList(it_out);

            _sender_buffer_id = getBufferID(_sender_buffer);

            i = gsl_matrix_get(_dispatcher_adjacency_matrix, _sender_buffer_id - 1, _dispatcher_id - 1);

            /* i = 1;*/

            /* fprintf(dispatcher_log, "sender %d receiver %d matrix_get %d\n", _sender_buffer_id, _dispatcher_id, i); */

            if (i == 1) {

                putInBuffer_blocking(_sender_buffer, _msg);

                fprintf(dispatcher_log, "D%d: > (%d,%d,%f,%f) > %d\n", _dispatcher_id, _msg->msg_id, _msg->msg_time, _msg->msg_x, _msg->msg_z, _sender_buffer_id);
            }
        }

        freeListIterator(it_out);

        fprintf(dispatcher_log, "D%d: ----------------------------------\n", _dispatcher_id);

        _msg = getFromBuffer_blocking(_dispatcher_buffer_in);
    }

    fprintf(dispatcher_log, "D%d: ------------ Loop End ------------\n", _dispatcher_id);

    /* 
     * Inoltra nel sender buffer associato e solo in quello il POISON_ID
     */

    it_out = allocListIterator(_dispatcher_lista_buffer_out);

    _sender_buffer = nextElementFromList(it_out);

    _sender_buffer_id = getBufferID(_sender_buffer);

    while (_sender_buffer_id != _dispatcher_id) {

        _sender_buffer = nextElementFromList(it_out);

        _sender_buffer_id = getBufferID(_sender_buffer);
    }

    putInBuffer_blocking(_sender_buffer, _msg);

    fprintf(dispatcher_log, "D%d: Signal:\n", _dispatcher_id);

    fprintf(dispatcher_log, "D%d: > snd (%d,%d,%f,%f) > %d\n", _dispatcher_id, _msg->msg_id, _msg->msg_time, _msg->msg_x, _msg->msg_z, _sender_buffer_id);

    fprintf(dispatcher_log, "D%d: Wait:\n", _dispatcher_id);

    fprintf(dispatcher_log, "D%d: > numero_receiver_vivi > 0\n", _dispatcher_id);


    /* 
     * Attende che tutti i receiver siano morti
     */

    while (numero_receiver_vivi > 0);

    fprintf(dispatcher_log, "D%d: Memoria:\n", _dispatcher_id);

    /* 
     * free delle variabili usate
     */

    freeBuffer(_dispatcher_buffer_in);

    fprintf(dispatcher_log, "D%d: > free(receiver_buffer)\n", _dispatcher_id);

    free(_parametri_dispatcher);

    fprintf(dispatcher_log, "D%d: > parametri\n", _dispatcher_id);

    /* 
     * Fa il fflush del file di log e lo chiude
     */

    fprintf(dispatcher_log, "D%d: Close log\n", _dispatcher_id);

    fflush(dispatcher_log);

    fclose(dispatcher_log);

    /* 
     * Decrementa numero_dispatcher_vivi e termina
     */

    numero_dispatcher_vivi--;

    return NULL;

}

parametri_router allocParamRouter(char *_router_ip_to_listen, int _port_client_to_router, int _port_router_to_client) {

    parametri_router _M_parametri_router = (struct parametri_router_t *) malloc(sizeof (struct parametri_router_t));

    _M_parametri_router->router_ip_to_listen = _router_ip_to_listen;
    _M_parametri_router->port_router_to_client = _port_router_to_client;
    _M_parametri_router->port_client_to_router = _port_client_to_router;

    return _M_parametri_router;
}

void *runRouter(void *_parametri_router) {

    parametri_router _parametri = (parametri_router) _parametri_router;

    char *_router_ip_to_listen;
    int _port_client_to_router;
    int _port_router_to_client;

    int _router_agents_number;

    list lista_buffer_in_M_;
    list_iterator it_in;
    buffer _buffer_in;

    list lista_buffer_out_M_;

    pthread_t dispatcher_T_;
    pthread_t accepter_in_T_;
    pthread_t accepter_out_T_;

    FILE *router_log;

    /* 
     * Apre il file di log
     */

    router_log = fopen("router.log", "w");

    if (router_log == NULL) {
        perror("Errore apertura file: \"router.log\"\n");
        exit(1);
    };

    setbuf(router_log, NULL);

    fprintf(router_log, ": Open log\n");

    fprintf(router_log, ": Parametri di invocazione:\n");

    _router_ip_to_listen = _parametri->router_ip_to_listen;

    fprintf(router_log, ": > _router_ip_to_listen = %s\n", _router_ip_to_listen);

    _port_client_to_router = _parametri->port_client_to_router;

    fprintf(router_log, ": > _port_client_to_router = %d\n", _port_client_to_router);

    _port_router_to_client = _parametri->port_router_to_client;

    fprintf(router_log, ": > _port_router_to_client = %d\n", _port_router_to_client);

    fprintf(router_log, ": Memoria:\n");

    _router_agents_number = AGENT_NUMBER;

    fprintf(router_log, ": > _router_agents_number = %d\n", _router_agents_number);

    lista_buffer_in_M_ = allocList();

    fprintf(router_log, ": > alloc(lista_buffer_in)\n");

    lista_buffer_out_M_ = allocList();

    fprintf(router_log, ": > alloc(lista_buffer_out)\n");

    /* 
     * Fa partire i due accepter (threads)
     */

    pthread_create(&accepter_in_T_, NULL, &runAccepter, allocParamAccepter(_router_agents_number, _router_ip_to_listen, _port_client_to_router, lista_buffer_in_M_, 0));

    fprintf(router_log, ": Thread -< Accepter():\n");

    fprintf(router_log, ": > _accepter_agents_number = %d\n", _router_agents_number);

    fprintf(router_log, ": > _accepter_address = %s\n", _router_ip_to_listen);

    fprintf(router_log, ": > _accepter_port = %d\n", _port_client_to_router);

    fprintf(router_log, ": > _accepter_in_out = %d\n", 0);

    pthread_create(&accepter_out_T_, NULL, &runAccepter, allocParamAccepter(_router_agents_number, _router_ip_to_listen, _port_router_to_client, lista_buffer_out_M_, 1));

    fprintf(router_log, ": Thread -< Accepter():\n");

    fprintf(router_log, ": > _accepter_agents_number = %d\n", _router_agents_number);

    fprintf(router_log, ": > _accepter_address = %s\n", _router_ip_to_listen);

    fprintf(router_log, ": > _accepter_port = %d\n", _port_router_to_client);

    fprintf(router_log, ": > _accepter_in_out = %d\n", 1);

    /* 
     * Attende che i due accepter abbiano terminato il loro compito
     */

    fprintf(router_log, ": Wait:\n");

    fprintf(router_log, ": > pthread_join(accepter_out)\n");

    pthread_join(accepter_out_T_, NULL);

    fprintf(router_log, ": > pthread_join(accepter_in)\n");

    pthread_join(accepter_in_T_, NULL);

    fprintf(router_log, ": Loop:\n");

    fprintf(router_log, ": > while hasNext(lista_buffer_in)\n");

    /* 
     * Associa ad ogni receiver buffer un dispatcher
     */

    it_in = allocListIterator(lista_buffer_in_M_);

    fprintf(router_log, ": ----------- Loop Start -----------\n");

    while (hasNextList(it_in) == 1) {

        _buffer_in = nextElementFromList(it_in);

        numero_dispatcher_vivi++;

        pthread_create(&dispatcher_T_, NULL, &runDispatcher, allocParamDispatcher(_buffer_in, lista_buffer_out_M_));

        fprintf(router_log, ": >>> create dispatcher thread\n");
    }

    freeListIterator(it_in);

    fprintf(router_log, ": ------------ Loop End ------------\n");

    /* Attende che tutti i sender thread creati siano morti, infatti loro sono gli ultimi a morire */

    fprintf(router_log, ": Wait:\n");

    fprintf(router_log, ": > numero_sender_vivi > 0\n");

    while (numero_sender_vivi > 0);

    fprintf(router_log, ": Memoria:\n");

    /*
     * free delle variabili usate
     */

    freeList(lista_buffer_in_M_);
    lista_buffer_in_M_ = NULL;

    fprintf(router_log, ": > free(lista_buffer_in)\n");

    freeList(lista_buffer_out_M_);
    lista_buffer_out_M_ = NULL;

    fprintf(router_log, ": > free(lista_buffer_out)\n");

    free(_parametri_router);

    fprintf(router_log, ": > parametri\n");

    /*
     * fflush e chiusura del logfile
     */

    fprintf(router_log, ": Close log\n");

    fflush(router_log);

    fclose(router_log);

    return NULL;
}

