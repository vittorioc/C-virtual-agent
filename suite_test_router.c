#include <pthread.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/CUCurses.h>
#include "router.h"
#include "suite_test_router.h"

#define ROUTER_ADDRESS "127.0.0.1"
#define PORT_IN 9000
#define PORT_OUT 4000

int initTestRouter(void) {

    return 0;
}

int cleanTestRouter(void) {

    return 0;
}

void test_router(void) {

/*
        pid_t pd1, pd2;

        pd1 = fork();

        if (pd1 == 0) {
            system("xterm -T Provider -hold -geometry +10+50 -e ./start_router 127.0.0.1 4000 5000");
            exit(0);
        }

        pd2 = fork();

        if (pd2 == 0) {
            system("xterm -T Client1 -hold -geometry +10+500 -e ./start_client 127.0.0.1 4000 5000");
            exit(0);
        }

        pd3 = fork();

        if (pd3 == 0) {
            system("xterm -T Client2 -hold -geometry +512+50 -e ./start_client 127.0.0.1 9000 5 50");
            exit(0);
        }

        pd4 = fork();

        if (pd4 == 0) {
            system("xterm -T Client3 -hold -geometry +512+500 -e ./start_client 127.0.0.1 9000 3 80");
            exit(0);
        }
     */
}
