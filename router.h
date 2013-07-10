#ifndef _ROUTER_H
#define	_ROUTER_H

#include "buffer.h"
#include "list.h"

/*
 * Attenzione: il valore -3 non può essere usato
 */

#define POISON_PILL -3

typedef struct router_t *router;

router allocRouter();
void freeRouter(router router_p);

typedef struct parametri_router_t {
    char *router_ip_to_listen;
    int port_router_to_client;
    int port_client_to_router;
} *parametri_router;

parametri_router allocParamRouter(char *_router_ip_to_listen, int _port_client_to_router, int _port_router_to_client);
void *runRouter(void *_parametri_router);

#endif	/* _ROUTER_H */


