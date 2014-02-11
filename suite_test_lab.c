#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/CUCurses.h>
#include "suite_test_lab.h"

int initTestLab(void) {

    return 0;
}

int cleanTestLab(void) {

    return 0;
}

void test_lab(void) {

    pid_t rtr_log, rtr;

    pid_t agt_log, agt1, agt2;

    int MAC = 1;

    rtr_log = fork();
    if (rtr_log == 0) {

        if (MAC) {
            system("xterm -geometry 160x80+0+0 -e 'multitail -x ROUTER -sw 38,41,41,41 -sn 3,2,2,2 --retry-all -M 10000 router.log accepter_in.log accepter_out.log receiver_1.log receiver_2.log dispatcher_1.log dispatcher_2.log sender_1.log sender_2.log'");
        } else {
            system("gnome-terminal --zoom=0.7 --hide-menubar --geometry 160x80+0+0 -e 'multitail -x ROUTER -sw 38,41,41,41 -sn 3,2,2,2 --retry-all -M 10000 router.log accepter_in.log accepter_out.log receiver_1.log receiver_2.log dispatcher_1.log dispatcher_2.log sender_1.log sender_2.log'");
        }

        exit(0);
    }

    agt_log = fork();
    if (agt_log == 0) {

        if (MAC) {
            system("xterm -geometry 80x80+885+0 -e 'multitail -x AGENTS --retry-all -M 10000 agent_1.log agent_2.log'");
        } else {
            system("gnome-terminal --zoom=0.7 --hide-menubar --geometry=80x80+885+0 -e 'multitail -x AGENTS --retry-all -M 10000 agent_1.log agent_2.log'");
        }

        exit(0);
    }

    rtr = fork();
    if (rtr == 0) {
        system("./start_router 127.0.0.1 1000 3000");
        exit(0);
    }

    sleep(2); /* Bisogna aspettare che gli accepter siano in listening */

    agt1 = fork();
    if (agt1 == 0) {
        system("./start_agent 1 127.0.0.1 1000 3000");
        exit(0);
    }

    agt2 = fork();
    if (agt2 == 0) {
        system("./start_agent 2 127.0.0.1 1000 3000");
        exit(0);
    }
}