#ifndef _LAB_H
#define	_LAB_H

#include <pthread.h>
#include "list.h"
#include "agent.h"

void strobeUp();
void strobeDown();
void addLink(agent agent1, agent agent2);
void removeLink(agent agent1, agent agent2);
void plotNetworkGraph(list network);
void plotNetworkFFT(list network);
void plotAgentFFT(agent agent);
void plotTimedAgentFFT(agent agent);

#endif	/* _LAB_H */

