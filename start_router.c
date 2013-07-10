#include <stdio.h>
#include <stdlib.h>
#include "router.h"

int main(int argc, char** argv) {

    if (argc != 4) {
        printf("Uso: start_router <router_ip> <port_in> <port_out>\n");
        exit(0);
    }

    runRouter(allocParamRouter(argv[1],atoi(argv[2]),atoi(argv[3])));

    return (EXIT_SUCCESS);
}