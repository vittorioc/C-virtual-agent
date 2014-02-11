/* usleep, inet_aton */
#define _GNU_SOURCE

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "agent.h"

/*
 * Grafo con 6 nodi -> dmax = 5
 * devo prendere una frequenza  fs > 2*dmax/pi
 * e quindi una ts < pi/(2*dmax) = 0.314 sec
 * prendiamo h di runge kutta = ts
 */

int _agents_number = 2;

int _neighborhood_array[2] = {1, 1};

float h = 0.1;

int N = 32768;

typedef enum {
    log_debug = 1,
    log_normal = 2,
    log_details = 4
} log_t;

int log_level = log_normal;

/*int log_level = log_debug | log_normal | log_details;*/

struct msg_t {
    int msg_id;
    int msg_time;
    float msg_x;
    float msg_z;
};

struct agent_t {
    int agent_id;
    int agent_x;
    int agent_z;
    pthread_mutex_t agent_mutex;
};

typedef struct msg_t *msg;

static pthread_mutex_t agent_class_mutex;
static int agentObjectsNumber = 0;

static void initClassAgent() {
    pthread_mutex_init(&(agent_class_mutex), NULL);
}

static void cleanClassAgent() {
    pthread_mutex_destroy(&(agent_class_mutex));

}

agent allocAgent() {

    agent agent_M_;

    if (agentObjectsNumber == 0) initClassAgent();

    agentObjectsNumber++;

    agent_M_ = (agent) malloc(sizeof (struct agent_t));

    agent_M_->agent_id = 0;
    pthread_mutex_init(&(agent_M_->agent_mutex), NULL);

    return agent_M_;

}

void freeAgent(agent _F_agent) {

    pthread_mutex_destroy(&(_F_agent->agent_mutex));

    free(_F_agent);

    _F_agent = NULL;

    agentObjectsNumber--;

    if (agentObjectsNumber == 0) cleanClassAgent();
}

void setIdAgent(agent agent_p) {
    agent_p = NULL;
}

void getIdAgent(agent agent_p) {
    agent_p = NULL;
}

void setAgentState(agent agent_p) {
    agent_p = NULL;
}

void getAgentState(agent agent_p) {
    agent_p = NULL;
}

static int connectTo(char *_ip_address_remote, int _ip_port_remote) {

    int socket_M_;

    struct sockaddr_in sockaddr_in_remote_;

    memset(&sockaddr_in_remote_, 0, sizeof (sockaddr_in_remote_));
    sockaddr_in_remote_.sin_family = AF_INET;
    sockaddr_in_remote_.sin_port = htons(_ip_port_remote);
    inet_aton(_ip_address_remote, &(sockaddr_in_remote_.sin_addr));

    if ((socket_M_ = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("connectTo: socket() Error\n");
        exit(1);
    }

    while (connect(socket_M_, (struct sockaddr *) & sockaddr_in_remote_, sizeof (struct sockaddr_in)) == -1) {
        perror("connectTo: connect() Error\n");
        exit(1);
    }

    return socket_M_;
}

static void sendMsg(int _socket, msg _msg) {

    if (send(_socket, _msg, sizeof (struct msg_t), 0) == -1) {
        perror("sendMsg: send() Error\n");
        exit(1);
    };
}

static msg recvMsg(int _socket) {

    msg _M_msg = (struct msg_t *) malloc(sizeof (struct msg_t));

    if (recv(_socket, _M_msg, sizeof (struct msg_t), 0) == -1) {
        perror("recvMsg: recv() Error\n");
        exit(1);
    }

    return _M_msg;
}

static int neighborhood(int _id) {

    int _neighborhood;

    _neighborhood = _neighborhood_array[_id - 1];

    return _neighborhood;
}

parametri_agent allocParamAgent(int _identity, char *_agent_router_ip, int _port_agent_to_router, int _port_router_to_agent) {

    parametri_agent _M_parametri_agent = (parametri_agent) malloc(sizeof (struct parametri_agent_t));

    _M_parametri_agent->identity = _identity;
    _M_parametri_agent->agent_router_ip = _agent_router_ip;
    _M_parametri_agent->port_router_to_agent = _port_router_to_agent;
    _M_parametri_agent->port_agent_to_router = _port_agent_to_router;

    return _M_parametri_agent;
}

void *runAgent(void *_parametri_agent) {

    parametri_agent _parametri = (parametri_agent) _parametri_agent;

    int _identity = _parametri->identity;

    char *_agent_router_ip;
    int _port_router_to_agent;
    int _port_agent_to_router;

    int agent_to_router_socket_;
    int router_to_agent_socket_;

    int _agent_neighborhood;

    msg _M_msg_in, _M_msg_out;

    double x[4], z[4];

    double sx, sz;

    int c, r, i;
    
    FILE *agent_log;
    char agent_log_file_name[FILENAME_MAX];

    /*
     * Apertura del file di log
     */

    sprintf(agent_log_file_name, "agent_%d.log", _identity);

    agent_log = fopen(agent_log_file_name, "w");

    if (agent_log == NULL) {
        perror("Errore apertura file: \"agent.log\"\n");
        exit(1);
    };

    setbuf(agent_log, NULL);

    if (log_level & log_debug) fprintf(agent_log, "A%d: Open log\n", _identity);

    if (log_level & log_debug) fprintf(agent_log, "A%d: Parametri di invocazione:\n", _identity);

    if (log_level & log_debug) fprintf(agent_log, "A%d: > _identity = %d\n", _identity, _identity);

    _agent_router_ip = _parametri->agent_router_ip;

    if (log_level & log_debug) fprintf(agent_log, "A%d: > _agent_router_ip = %s\n", _identity, _agent_router_ip);

    _port_agent_to_router = _parametri->port_agent_to_router;

    if (log_level & log_debug) fprintf(agent_log, "A%d: > _port_agent_to_router = %d\n", _identity, _port_agent_to_router);

    _port_router_to_agent = _parametri->port_router_to_agent;

    if (log_level & log_debug) fprintf(agent_log, "A%d: > _port_router_to_agent = %d\n", _identity, _port_router_to_agent);

    if (log_level & log_debug) fprintf(agent_log, "A%d: Memoria:\n", _identity);

    if (log_level & log_debug) fprintf(agent_log, "A%d: > h = %2.8g\n", _identity, h);

    /*
     * Calcolo del numero di agent vicini attraverso l'analisi della adjacency_matrix
     */

    _agent_neighborhood = neighborhood(_identity);

    if (log_level & log_debug) fprintf(agent_log, "A%d: > _agent_neighborhood = %d\n", _identity, _agent_neighborhood);

    if (log_level & log_debug) fprintf(agent_log, "A%d: > _agents_number = %d\n", _identity, _agents_number);

    _M_msg_out = (struct msg_t *) malloc(sizeof (struct msg_t));

    if (log_level & log_debug) fprintf(agent_log, "A%d: > alloc(msg_out)\n", _identity);

    if (log_level & log_debug) fprintf(agent_log, "A%d: Socket:\n", _identity);

    agent_to_router_socket_ = connectTo(_agent_router_ip, _port_agent_to_router);

    if (log_level & log_debug) fprintf(agent_log, "A%d: > connectTo(%s,%d)\n", _identity, _agent_router_ip, _port_agent_to_router);

    router_to_agent_socket_ = connectTo(_agent_router_ip, _port_router_to_agent);

    if (log_level & log_debug) fprintf(agent_log, "A%d: > connectTo(%s,%d)\n", _identity, _agent_router_ip, _port_router_to_agent);

    if (log_level & log_debug) fprintf(agent_log, "A%d: Signal:\n", _identity);

    /*
     * Imposto l'identità dei messaggi in output
     */

    _M_msg_out->msg_id = _identity;

    /*
     * Imposto il timer interno
     */

    _M_msg_out->msg_time = 0;

    /*
     * Invio di un messaggio contenente la propria identità sui due canali di in/out
     */

    _M_msg_out->msg_x = 0.00;
    _M_msg_out->msg_z = 0.00;

    sendMsg(router_to_agent_socket_, _M_msg_out);

    if (log_level & log_debug) fprintf(agent_log, "A%d: > snd (%d,%d,%2.8g,%2.8g) > %d\n", _identity, _M_msg_out->msg_id, _M_msg_out->msg_time, _M_msg_out->msg_x, _M_msg_out->msg_z, _port_router_to_agent);

    sendMsg(agent_to_router_socket_, _M_msg_out);

    if (log_level & log_debug) fprintf(agent_log, "A%d: > snd (%d,%d,%2.8g,%2.8g) > %d\n", _identity, _M_msg_out->msg_id, _M_msg_out->msg_time, _M_msg_out->msg_x, _M_msg_out->msg_z, _port_agent_to_router);

    /* goto loopend; */

    /*
     * Ciclo principale
     */

    if (log_level & log_debug) fprintf(agent_log, "A%d: Loop:\n", _identity);

    if (log_level & log_debug) fprintf(agent_log, "A%d: > for i=[0,%d]\n", _identity, N);

    if (log_level & log_debug) fprintf(agent_log, "A%d: ----- Loop Start -----\n", _identity);

       if (_identity == 1) {

        x[0] = 1.00;
        z[0] = 0.00;

    } else {

        x[0] = 0.00;
        z[0] = 0.00;

    }

    x[1] = 0.00;
    x[2] = 0.00;
    x[3] = 0.00;

    z[1] = 0.00;
    z[2] = 0.00;
    z[3] = 0.00;

    sx = 0.00;
    sz = 0.00;
    r = 0;

    for (c = 0; c < N; c++) {

        if (log_level & log_debug) fprintf(agent_log, "A%d: > i = %d\n", _identity, c);

        fprintf(agent_log, "%2.8g %2.8g\n", x[0], z[0]);
        
        for (i = 0; i < 5; i++) {
            /*
             * snd x
             */

            _M_msg_out->msg_time = i;

            _M_msg_out->msg_x = x[i];
            _M_msg_out->msg_z = z[i];

            sendMsg(agent_to_router_socket_, _M_msg_out);

            
            if (log_level & log_details) fprintf(agent_log, "A%d: > snd (%d,%d,%2.8g,%2.8g)\n", _identity, _M_msg_out->msg_id, _M_msg_out->msg_time, _M_msg_out->msg_x, _M_msg_out->msg_z);

            /*
             * k1
             */

            while (r < _agent_neighborhood) {

                _M_msg_in = recvMsg(router_to_agent_socket_);

                if (log_level & log_details) fprintf(agent_log, "A%d: > rcv (%d,%d,%2.8g,%2.8g)\n", _identity, _M_msg_in->msg_id, _M_msg_out->msg_time, _M_msg_in->msg_x, _M_msg_in->msg_z);

                sx = sx + _M_msg_in->msg_x - x[i];
                sz = sz + _M_msg_in->msg_z - z[i];
                r++;
            }


            switch (i) {

                case 0:
                    x[1] = -sz;
                    z[1] = sx;
                    break;

                case 1:
                    x[2] = x[1] - h * sz / 2;
                    z[2] = z[1] + h * sx / 2;
                    break;

                case 2:
                    x[3] = x[1] - h * sz / 2;
                    z[3] = z[1] + h * sx / 2;
                    break;

                case 3:
                    x[0] = x[0] + h * (x[1] + x[2] + x[3] - h * sz / 2) / 3;
                    z[0] = z[0] + h * (z[1] + z[2] + z[3] + h * sx / 2) / 3;
                    break;

            }

            sx = 0;
            sz = 0;
            r = 0;

        }
    }

    if (log_level & log_debug) fprintf(agent_log, "A%d: ----- Loop End ------\n", _identity);

    /* loopend: */

    if (log_level & log_debug) fprintf(agent_log, "A%d: Signal:\n", _identity);

    /*
     * Invio del killer message
     */

    _M_msg_out->msg_id = -1;
    _M_msg_out->msg_x = 0;
    _M_msg_out->msg_z = 0;

    sendMsg(agent_to_router_socket_, _M_msg_out);

    if (log_level & log_debug) fprintf(agent_log, "A%d: > snd (%d,%d,%2.8g,%2.8g) > %d\n", _identity, _M_msg_out->msg_id, _M_msg_out->msg_time, _M_msg_out->msg_x, _M_msg_out->msg_z, _port_agent_to_router);

    if (log_level & log_debug) fprintf(agent_log, "A%d: Socket:\n", _identity);

    /*
     * Chiusura delle sockets
     */

    close(router_to_agent_socket_);

    if (log_level & log_debug) fprintf(agent_log, "A%d: > close(%d)\n", _identity, _port_router_to_agent);

    close(agent_to_router_socket_);

    if (log_level & log_debug) fprintf(agent_log, "A%d: > close(%d)\n", _identity, _port_agent_to_router);

    if (log_level & log_debug) fprintf(agent_log, "A%d: Memoria:\n", _identity);

    /*
     * Free delle variabili usate
     */

    free(_M_msg_out);

    if (log_level & log_debug) fprintf(agent_log, "A%d: > free(msg_out)\n", _identity);

    free(_parametri_agent);

    if (log_level & log_debug) fprintf(agent_log, "A%d: > parametri\n", _identity);

    /*
     * fflush e chiusura del logfile
     */

    if (log_level & log_debug) fprintf(agent_log, "A%d: Close log\n", _identity);

    fflush(agent_log);

    fclose(agent_log);

    return NULL;
}