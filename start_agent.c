#include <stdio.h>
#include <stdlib.h>
#include "agent.h"

int main(int argc, char** argv) {

    if (argc != 5) {
        printf("Uso: start_agent <identity> <router_ip> <port_in> <port_out>\n");
        exit(0);
    }

    runAgent(allocParamAgent(atoi(argv[1]),argv[2],atoi(argv[3]),atoi(argv[4])));

    return (EXIT_SUCCESS);
}
