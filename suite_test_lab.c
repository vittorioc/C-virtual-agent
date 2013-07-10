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

    pid_t agt_log, agt1, agt2, agt3, agt4, agt5, agt6;

    int MAC = 1;

    rtr_log = fork();
    if (rtr_log == 0) {

        if (MAC) {
            system("xterm -geometry 160x80+0+0 -e 'multitail -x ROUTER -sw 38,41,41,41 -sn 3,6,6,6 --retry-all -M 10000 router.log accepter_in.log accepter_out.log receiver_1.log receiver_2.log receiver_3.log receiver_4.log receiver_5.log receiver_6.log dispatcher_1.log dispatcher_2.log dispatcher_3.log dispatcher_4.log dispatcher_5.log dispatcher_6.log sender_1.log sender_2.log sender_3.log sender_4.log sender_5.log sender_6.log'");
        } else {
            system("gnome-terminal --zoom=0.7 --hide-menubar --geometry 160x80+0+0 -e 'multitail -x ROUTER -sw 38,41,41,41 -sn 3,6,6,6 --retry-all -M 10000 router.log accepter_in.log accepter_out.log receiver_1.log receiver_2.log receiver_3.log receiver_4.log receiver_5.log receiver_6.log dispatcher_1.log dispatcher_2.log dispatcher_3.log dispatcher_4.log dispatcher_5.log dispatcher_6.log sender_1.log sender_2.log sender_3.log sender_4.log sender_5.log sender_6.log'");
        }

        exit(0);
    }

    agt_log = fork();
    if (agt_log == 0) {

        if (MAC) {
            system("xterm -geometry 80x80+885+0 -e 'multitail -x AGENTS --retry-all -M 10000 agent_1.log agent_2.log agent_3.log agent_4.log agent_5.log agent_6.log'");
        } else {
            system("gnome-terminal --zoom=0.7 --hide-menubar --geometry=80x80+885+0 -e 'multitail -x AGENTS --retry-all -M 10000 agent_1.log agent_2.log agent_3.log agent_4.log agent_5.log agent_6.log'");
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

    agt3 = fork();
    if (agt3 == 0) {
        system("./start_agent 3 127.0.0.1 1000 3000");
        exit(0);
    }

    agt4 = fork();
    if (agt4 == 0) {
        system("./start_agent 4 127.0.0.1 1000 3000");
        exit(0);
    }

    agt5 = fork();
    if (agt5 == 0) {
        system("./start_agent 5 127.0.0.1 1000 3000");
        exit(0);
    }

    agt6 = fork();
    if (agt6 == 0) {
        system("./start_agent 6 127.0.0.1 1000 3000");
        exit(0);
    }
}