#ifndef _AGENT_H
#define	_AGENT_H

/*
 * Attenzione: il valore -3 non può essere usato
 */

#define POISON_PILL -3

typedef struct agent_t *agent;

agent allocAgent();
void freeAgent(agent agent_p);
void setIdAgent(agent agent_p);
void getIdAgent(agent agent_p);
void setAgentState(agent agent_p);
void getAgentState(agent agent_p);

typedef struct parametri_agent_t {
    int identity;
    char *agent_router_ip;
    int port_router_to_agent;
    int port_agent_to_router;
} *parametri_agent;

parametri_agent allocParamAgent(int identity, char *_agent_router_ip, int _port_agent_to_router, int _port_router_to_agent);
void *runAgent(void *_parametri_agent);

#endif	/* _AGENT_H */

