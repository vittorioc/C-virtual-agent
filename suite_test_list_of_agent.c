#include <pthread.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/CUCurses.h>
#include "agent.h"
#include "list.h"

struct network_element_t {
    struct agent_t* agent_p;
    struct network_element_t* network_element_prev;
    struct network_element_t* network_element_succ;
};

struct network_t {
    struct network_element_t* network_element_head;
    struct network_element_t* network_element_tail;
    pthread_mutex_t network_mutex;
};

struct network_iterator_t {
    struct network_t* network_p;
    struct network_element_t* network_element_next;
    pthread_mutex_t network_iterator_mutex;
};

typedef struct network_element_t *network_element;

struct list_element_t {
    element element_pointer;
    struct list_element_t *list_element_prev;
    struct list_element_t *list_element_succ;
};

struct list_t {
    struct list_element_t *list_element_head;
    struct list_element_t *list_element_tail;
    pthread_mutex_t list_mutex;
};

struct list_iterator_t {
    struct list_t *list_pointer;
    struct list_element_t *list_element_next;
};

typedef struct list_element_t *list_element;

/*
 * Si fa l'ipotesi che la funzione
 * freeNetwork
 * sia corretta
 */

int initTestListOfAgent(void) {

    return 0;
}

int cleanTestListOfAgent(void) {

    return 0;
}

void test_Agent_allocList(void) {

    list list_;

    list_ = allocList();

    CU_ASSERT_PTR_NOT_EQUAL(list_, NULL);

    CU_ASSERT_PTR_EQUAL(list_->list_element_tail, NULL);

    CU_ASSERT_PTR_EQUAL(list_->list_element_head, NULL);

    freeList(list_);
}

void test_Agent_addElementToList_lista_vuota(void) {

    agent agent_ = allocAgent();

    list list_ = allocList();

    addElementToList(list_, agent_);

    CU_ASSERT_PTR_EQUAL(list_->list_element_head->list_element_prev, NULL);

    CU_ASSERT_PTR_EQUAL(list_->list_element_head->element_pointer, agent_);

    CU_ASSERT_PTR_EQUAL(list_->list_element_head->list_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(list_->list_element_tail, list_->list_element_head);

    freeList(list_);

    freeAgent(agent_);
}

void test_Agent_addElementToList_di_due_agent_su_lista_vuota(void) {

    agent agent1_ = allocAgent();

    agent agent2_ = allocAgent();

    list list_ = allocList();

    addElementToList(list_, agent1_);

    addElementToList(list_, agent2_);

    CU_ASSERT_PTR_EQUAL(list_->list_element_head->list_element_prev, NULL);

    CU_ASSERT_PTR_EQUAL(list_->list_element_head->element_pointer, agent1_);

    CU_ASSERT_PTR_EQUAL(list_->list_element_head->list_element_succ, list_->list_element_tail);

    CU_ASSERT_PTR_EQUAL(list_->list_element_tail->list_element_prev, list_->list_element_head);

    CU_ASSERT_PTR_EQUAL(list_->list_element_tail->element_pointer, agent2_);

    CU_ASSERT_PTR_EQUAL(list_->list_element_tail->list_element_succ, NULL);

    freeList(list_);

    freeAgent(agent1_);

    freeAgent(agent2_);
}

void test_allocNetworkIterator_network_vuota(void) {

    /*
    network net_p = allocNetwork();

    network_iterator iter_p;

    iter_p = allocNetworkIterator(net_p);

    CU_ASSERT_PTR_EQUAL(iter_p->network_p, net_p);

    CU_ASSERT_PTR_EQUAL(iter_p->network_element_next, NULL);

    * Controllo che allocNetworkIterator non abbia side-effects sulla network *

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail, NULL);

    freeNetworkIterator(iter_p);

    freeNetwork(net_p);*/
}

void test_freeNetworkIterator_network_vuota(void) {
/*
    network net_p = allocNetwork();

    network_iterator iter_p = allocNetworkIterator(net_p);

    freeNetworkIterator(iter_p);

    * Controllo che freeNetworkIterator non abbia side-effects sulla network *

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail, NULL);

    freeNetwork(net_p);
 */
}

void test_allocNetworkIterator_network_un_agent(void) {
/*
    agent agent_p = allocAgent();

    network net_p = allocNetwork();

    network_iterator iter_p;

    addAgentToNetwork(net_p, agent_p);

    iter_p = allocNetworkIterator(net_p);

    CU_ASSERT_PTR_EQUAL(iter_p->network_p, net_p);

    CU_ASSERT_PTR_EQUAL(iter_p->network_element_next->agent_p, net_p->network_element_head->agent_p);

    * Controllo che allocNetworkIterator non abbia side-effects sulla network *

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->network_element_prev, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->agent_p, agent_p);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->network_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail, net_p->network_element_head);

    freeNetworkIterator(iter_p);

    freeNetwork(net_p);

    freeAgent(agent_p);
 */
}

void test_freeNetworkIterator_network_un_agent(void) {
/*
    agent agent_p = allocAgent();

    network net_p = allocNetwork();

    network_iterator iter_p;

    addAgentToNetwork(net_p, agent_p);

    iter_p = allocNetworkIterator(net_p);

    freeNetworkIterator(iter_p);

    * Controllo che freeNetworkIterator non abbia side-effects sulla network *

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->network_element_prev, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->agent_p, agent_p);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->network_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail, net_p->network_element_head);

    freeNetwork(net_p);

    freeAgent(agent_p);
 */
}

void test_hasNext_network_vuota(void) {
/*
    network net_p = allocNetwork();

    network_iterator iter_p = allocNetworkIterator(net_p);

    CU_ASSERT_EQUAL(hasNext(iter_p), 0);

    CU_ASSERT_PTR_EQUAL(iter_p->network_p, net_p);

    CU_ASSERT_PTR_EQUAL(iter_p->network_element_next, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail, NULL);

    freeNetworkIterator(iter_p);

    freeNetwork(net_p);
*/
}

void test_hasNext_network_un_agent(void) {
/*
    agent agent_p = allocAgent();

    network net_p = allocNetwork();

    network_iterator iter_p;

    addAgentToNetwork(net_p, agent_p);

    iter_p = allocNetworkIterator(net_p);

    CU_ASSERT_EQUAL(hasNext(iter_p), 1);

    CU_ASSERT_PTR_EQUAL(iter_p->network_p, net_p);

    CU_ASSERT_PTR_EQUAL(iter_p->network_element_next->agent_p, net_p->network_element_head->agent_p);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->network_element_prev, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->agent_p, agent_p);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->network_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail, net_p->network_element_head);

    freeNetworkIterator(iter_p);

    freeNetwork(net_p);

    freeAgent(agent_p);
  */
}

void test_next_network_vuota(void) {
/*
    network net_p = allocNetwork();

    network_iterator iter_p = allocNetworkIterator(net_p);

    CU_ASSERT_PTR_EQUAL(next(iter_p), NULL);

    CU_ASSERT_PTR_EQUAL(iter_p->network_p, net_p);

    CU_ASSERT_PTR_EQUAL(iter_p->network_element_next, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail, NULL);

    freeNetworkIterator(iter_p);

    freeNetwork(net_p);
*/
}

void test_next_network_un_agent(void) {
/*
    agent agent_p = allocAgent();

    network net_p = allocNetwork();

    network_iterator iter_p;

    addAgentToNetwork(net_p, agent_p);

    iter_p = allocNetworkIterator(net_p);

    CU_ASSERT_PTR_EQUAL(next(iter_p), agent_p);

    CU_ASSERT_PTR_EQUAL(iter_p->network_p, net_p);

    CU_ASSERT_PTR_EQUAL(iter_p->network_element_next, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->network_element_prev, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->agent_p, agent_p);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->network_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail, net_p->network_element_head);

    freeNetworkIterator(iter_p);

    freeNetwork(net_p);

    freeAgent(agent_p);
 */
}

void test_next_network_due_agent(void) {
/*
    agent agent1_p = allocAgent();

    agent agent2_p = allocAgent();

    network net_p = allocNetwork();

    network_iterator iter_p;

    addAgentToNetwork(net_p, agent1_p);

    addAgentToNetwork(net_p, agent2_p);

    iter_p = allocNetworkIterator(net_p);

    CU_ASSERT_PTR_EQUAL(next(iter_p), agent1_p);

    CU_ASSERT_PTR_EQUAL(iter_p->network_p, net_p);

    CU_ASSERT_PTR_EQUAL(iter_p->network_element_next->agent_p, net_p->network_element_tail->agent_p);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->network_element_prev, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->agent_p, agent1_p);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->network_element_succ, net_p->network_element_tail);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail->network_element_prev, net_p->network_element_head);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail->agent_p, agent2_p);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail->network_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(next(iter_p), agent2_p);

    CU_ASSERT_PTR_EQUAL(iter_p->network_p, net_p);

    CU_ASSERT_PTR_EQUAL(iter_p->network_element_next, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->network_element_prev, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->agent_p, agent1_p);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->network_element_succ, net_p->network_element_tail);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail->network_element_prev, net_p->network_element_head);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail->agent_p, agent2_p);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail->network_element_succ, NULL);

    freeNetworkIterator(iter_p);

    freeNetwork(net_p);

    freeAgent(agent1_p);

    freeAgent(agent2_p);
 * */
}

void test_next_network_tutti_gli_agent(void) {
/*
    int numero_agenti = 10;

    agent agent_p[10];

    network net_p = allocNetwork();

    network_iterator iter_p;

    int i, k;

    for (i = 0; i < numero_agenti; i++) agent_p[i] = allocAgent();

    for (i = 0; i < numero_agenti; i++) addAgentToNetwork(net_p, agent_p[i]);

    iter_p = allocNetworkIterator(net_p);

    i = 0;

    while (hasNext(iter_p) == 1) {

        network_element temp_element_p;

        CU_ASSERT_PTR_EQUAL(next(iter_p), agent_p[i]);

        * Controllo che la next aggiorni corretamente l'network_iteratore al prossimo agente *

        CU_ASSERT_PTR_EQUAL(iter_p->network_p, net_p);

        if (i < 9) {

            CU_ASSERT_PTR_EQUAL(iter_p->network_element_next->agent_p, agent_p[i + 1])

        } else {

            CU_ASSERT_PTR_EQUAL(iter_p->network_element_next, NULL)

        };

        * Controllo che la next non modifichi la struttura della network che deve rimanere invariata *

        temp_element_p = net_p->network_element_head;

        k = 0;

        while (k < 10) {

            if (k == 0) {

                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_prev, NULL);

            } else {

                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_prev->agent_p, agent_p[k - 1]);

            }

            CU_ASSERT_PTR_EQUAL(temp_element_p->agent_p, agent_p[k]);

            if (k == 9) {

                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_succ, NULL);

            } else {

                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_succ->agent_p, agent_p[k + 1]);

            }

            temp_element_p = temp_element_p->network_element_succ;

            k++;

        }

        i++;
    }

    freeNetworkIterator(iter_p);

    freeNetwork(net_p);

    for (i = 0; i < numero_agenti; i++) freeAgent(agent_p[i]);
*/
}

void test_removeLastReturned_network_vuota(void) {
/*
    network net_p = allocNetwork();

    network_iterator iter_p = allocNetworkIterator(net_p);

    next(iter_p);

    removeLastReturned(iter_p);

    CU_ASSERT_PTR_EQUAL(iter_p->network_p, net_p);

    CU_ASSERT_PTR_EQUAL(iter_p->network_element_next, NULL);

    * Controllo che removeLastReturned non abbia side-effects sulla network *

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail, NULL);

    freeNetworkIterator(iter_p);

    freeNetwork(net_p);
 * */

}

void test_removeLastReturned_network_un_agent(void) {
/*
    agent agent_p = allocAgent();

    network net_p = allocNetwork();

    network_iterator iter_p;

    addAgentToNetwork(net_p, agent_p);

    iter_p = allocNetworkIterator(net_p);

    next(iter_p);

    removeLastReturned(iter_p);

    * Controllo che removeLastReturned modifichi correttamente la network *

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail, NULL);

    freeNetworkIterator(iter_p);

    freeNetwork(net_p);

    freeAgent(agent_p);
 * */
}

void test_removeLastReturned_primo_di_due_agent(void) {
/*
    agent agent1_p = allocAgent();

    agent agent2_p = allocAgent();

    network net_p = allocNetwork();

    network_iterator iter_p;

    addAgentToNetwork(net_p, agent1_p);

    addAgentToNetwork(net_p, agent2_p);

    iter_p = allocNetworkIterator(net_p);

    next(iter_p);

    removeLastReturned(iter_p);

    * Controllo che removeLastReturned modifichi correttamente la network *

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->network_element_prev, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->agent_p, agent2_p);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->network_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail, net_p->network_element_head);

    freeNetworkIterator(iter_p);

    freeNetwork(net_p);

    freeAgent(agent1_p);

    freeAgent(agent2_p);
 * */

}

void test_removeLastReturned_secondo_di_due_agent(void) {
/*
    agent agent1_p = allocAgent();

    agent agent2_p = allocAgent();

    network net_p = allocNetwork();

    network_iterator iter_p;

    addAgentToNetwork(net_p, agent1_p);

    addAgentToNetwork(net_p, agent2_p);

    iter_p = allocNetworkIterator(net_p);

    next(iter_p);

    next(iter_p);

    removeLastReturned(iter_p);

    * Controllo che removeLastReturned modifichi correttamente la network *

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->network_element_prev, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->agent_p, agent1_p);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->network_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail, net_p->network_element_head);

    freeNetworkIterator(iter_p);

    freeNetwork(net_p);

    freeAgent(agent1_p);

    freeAgent(agent2_p);
 * */
}

void test_removeLastReturned_primo_di_molti_agent(void) {
/*
    int numero_agenti = 10;

    network_element temp_element_p;

    agent agent_p[10];

    network net_p = allocNetwork();

    network_iterator iter_p;

    int i;

    for (i = 0; i < numero_agenti; i++) agent_p[i] = allocAgent();

    for (i = 0; i < numero_agenti; i++) addAgentToNetwork(net_p, agent_p[i]);

    iter_p = allocNetworkIterator(net_p);

    next(iter_p);

    removeLastReturned(iter_p);

    * Controllo che la removeLastReturned modifichi correttamente la struttura della network *

    temp_element_p = net_p->network_element_head;

    i = 1;

    while (i < 10) {

        if (i == 1) {

            CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_prev, NULL);

        } else {

            CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_prev->agent_p, agent_p[i - 1]);

        }

        CU_ASSERT_PTR_EQUAL(temp_element_p->agent_p, agent_p[i]);

        if (i == 9) {

            CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_succ, NULL);

        } else {

            CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_succ->agent_p, agent_p[i + 1]);

        }

        temp_element_p = temp_element_p->network_element_succ;

        i++;

    }

    freeNetworkIterator(iter_p);

    freeNetwork(net_p);

    for (i = 0; i < numero_agenti; i++) freeAgent(agent_p[i]);
 * */
}

void test_removeLastReturned_ultimo_di_molti_agent(void) {
/*
    int numero_agenti = 10;

    network_element temp_element_p;

    agent agent_p[10];

    network net_p = allocNetwork();

    network_iterator iter_p;

    int i;

    for (i = 0; i < numero_agenti; i++) agent_p[i] = allocAgent();

    for (i = 0; i < numero_agenti; i++) addAgentToNetwork(net_p, agent_p[i]);

    iter_p = allocNetworkIterator(net_p);

    for (i = 0; i < numero_agenti; i++) next(iter_p);

    removeLastReturned(iter_p);

    * Controllo che la removeLastReturned modifichi correttamente la struttura della network *

    temp_element_p = net_p->network_element_head;

    i = 0;

    while (i < 9) {

        if (i == 0) {

            CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_prev, NULL);

        } else {

            CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_prev->agent_p, agent_p[i - 1]);

        }

        CU_ASSERT_PTR_EQUAL(temp_element_p->agent_p, agent_p[i]);

        if (i == 8) {

            CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_succ, NULL);

        } else {

            CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_succ->agent_p, agent_p[i + 1]);

        }

        temp_element_p = temp_element_p->network_element_succ;

        i++;

    }

    freeNetworkIterator(iter_p);

    freeNetwork(net_p);

    for (i = 0; i < numero_agenti; i++) freeAgent(agent_p[i]);
*/
}

void test_removeLastReturned_in_mezzo_a_molti_agent(void) {
/*
    int numero_agenti = 10;

    network_element temp_element_p;

    agent agent_p[10];

    network net_p = allocNetwork();

    network_iterator iter_p;

    int i;

    for (i = 0; i < numero_agenti; i++) agent_p[i] = allocAgent();

    for (i = 0; i < numero_agenti; i++) addAgentToNetwork(net_p, agent_p[i]);

    iter_p = allocNetworkIterator(net_p);

    for (i = 0; i < 7; i++) next(iter_p);

    removeLastReturned(iter_p);

    * Controllo che la removeLastReturned modifichi correttamente la struttura della network *

    temp_element_p = net_p->network_element_head;

    i = 0;

    while (i < 10) {

        switch (i) {

            case 0:
                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_prev, NULL);
                break;

            case 7:
                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_prev->agent_p, agent_p[5]);
                break;

            default:
                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_prev->agent_p, agent_p[i - 1]);
                break;
        }

        CU_ASSERT_PTR_EQUAL(temp_element_p->agent_p, agent_p[i]);

        switch (i) {

            case 5:
                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_succ->agent_p, agent_p[7]);
                i = 6; * Avendo eliminato il sesto elemento salto da i=5 a i=7*
                break;

            case 9:
                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_succ, NULL);
                break;

            default:
                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_succ->agent_p, agent_p[i + 1]);
                break;

        }

        temp_element_p = temp_element_p->network_element_succ;

        i++;

    }

    freeNetworkIterator(iter_p);

    freeNetwork(net_p);

    for (i = 0; i < numero_agenti; i++) freeAgent(agent_p[i]);
*/
}

void test_removeAgent_network_vuota(void) {
/*
    network net_p = allocNetwork();

    agent agent_p = allocAgent();

    removeAgent(net_p, agent_p);

    CU_ASSERT_PTR_NOT_EQUAL(net_p, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail, NULL);

    freeNetwork(net_p);

    freeAgent(agent_p);
 * */
}

void test_removeAgent_network_un_agent_differente(void) {
/*
    agent agent1_p = allocAgent();

    agent agent2_p = allocAgent();

    network net_p = allocNetwork();

    addAgentToNetwork(net_p, agent1_p);

    removeAgent(net_p, agent2_p);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->network_element_prev, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->agent_p, agent1_p);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head->network_element_succ, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail, net_p->network_element_head);

    freeNetwork(net_p);

    freeAgent(agent1_p);

    freeAgent(agent2_p);
 * */
}

void test_removeAgent_network_un_agent_uguale(void) {
/*
    agent agent_p = allocAgent();

    network net_p = allocNetwork();

    addAgentToNetwork(net_p, agent_p);

    removeAgent(net_p, agent_p);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_head, NULL);

    CU_ASSERT_PTR_EQUAL(net_p->network_element_tail, NULL);

    freeNetwork(net_p);

    freeAgent(agent_p);
 * */
}

void test_removeAgent_network_molti_agent_nessuno_uguale(void) {
/*
    int numero_agenti = 10;

    network_element temp_element_p;

    agent agent_p[10];

    agent agent_q = allocAgent();

    network net_p = allocNetwork();

    int i;

    for (i = 0; i < numero_agenti; i++) agent_p[i] = allocAgent();

    for (i = 0; i < numero_agenti; i++) addAgentToNetwork(net_p, agent_p[i]);

    removeAgent(net_p, agent_q);

   * Controllo che la removeLastAgent non modifichi la struttura della network *

    temp_element_p = net_p->network_element_head;

    i = 0;

    while (i < 10) {

        if (i == 0) {

            CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_prev, NULL);

        } else {

            CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_prev->agent_p, agent_p[i - 1]);

        }

        CU_ASSERT_PTR_EQUAL(temp_element_p->agent_p, agent_p[i]);

        if (i == 9) {

            CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_succ, NULL);

        } else {

            CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_succ->agent_p, agent_p[i + 1]);

        }

        temp_element_p = temp_element_p->network_element_succ;

        i++;

    }

    freeNetwork(net_p);

    for (i = 0; i < numero_agenti; i++) freeAgent(agent_p[i]);

    freeAgent(agent_q);
 * */
}

void test_removeAgent_network_molti_agent_uno_uguale(void) {
/*
    int numero_agenti = 10;

    network_element temp_element_p;

    agent agent_p[10];

    network net_p = allocNetwork();

    int i;

    for (i = 0; i < numero_agenti; i++) agent_p[i] = allocAgent();

    for (i = 0; i < numero_agenti; i++) addAgentToNetwork(net_p, agent_p[i]);

    removeAgent(net_p, agent_p[6]);

    * Controllo che la removeLastReturned modifichi correttamente la struttura della network *

    temp_element_p = net_p->network_element_head;

    i = 0;

    while (i < 10) {

        switch (i) {

            case 0:
                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_prev, NULL);
                break;

            case 7:
                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_prev->agent_p, agent_p[5]);
                break;

            default:
                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_prev->agent_p, agent_p[i - 1]);
                break;
        }

        CU_ASSERT_PTR_EQUAL(temp_element_p->agent_p, agent_p[i]);

        switch (i) {

            case 5:
                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_succ->agent_p, agent_p[7]);
                i = 6; * Avendo eliminato il sesto elemento salto da i=5 a i=7*
                break;

            case 9:
                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_succ, NULL);
                break;

            default:
                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_succ->agent_p, agent_p[i + 1]);
                break;

        }

        temp_element_p = temp_element_p->network_element_succ;

        i++;

    }

    freeNetwork(net_p);

    for (i = 0; i < numero_agenti; i++) freeAgent(agent_p[i]);
 * */

}

void test_removeAgent_network_molti_agent_uno_ripetuto(void) {
/*
    int numero_agenti = 10;

    network_element temp_element_p;

    agent agent_p[10];

    network net_p = allocNetwork();

    int i;

    for (i = 0; i < numero_agenti; i++) agent_p[i] = allocAgent();

    for (i = 0; i < numero_agenti; i++) addAgentToNetwork(net_p, agent_p[i]);

    addAgentToNetwork(net_p, agent_p[6]);

    addAgentToNetwork(net_p, agent_p[6]);

    removeAgent(net_p, agent_p[6]);

    * Controllo che la removeLastReturned modifichi correttamente la struttura della network *

    temp_element_p = net_p->network_element_head;

    i = 0;

    while (i < 10) {

        switch (i) {

            case 0:
                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_prev, NULL);
                break;

            case 7:
                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_prev->agent_p, agent_p[5]);
                break;

            default:
                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_prev->agent_p, agent_p[i - 1]);
                break;
        }

        CU_ASSERT_PTR_EQUAL(temp_element_p->agent_p, agent_p[i]);

        switch (i) {

            case 5:
                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_succ->agent_p, agent_p[7]);
                i = 6; * Avendo eliminato il sesto elemento salto da i=5 a i=7*
                break;

            case 9:
                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_succ->agent_p, agent_p[6]);
                break;

            default:
                CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_succ->agent_p, agent_p[i + 1]);
                break;

        }

        temp_element_p = temp_element_p->network_element_succ;

        i++;

    }

    CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_prev->agent_p, agent_p[9]);

    CU_ASSERT_PTR_EQUAL(temp_element_p->agent_p, agent_p[6]);

    CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_succ->agent_p, agent_p[6]);

    temp_element_p = temp_element_p->network_element_succ;

    CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_prev->agent_p, agent_p[6]);

    CU_ASSERT_PTR_EQUAL(temp_element_p->agent_p, agent_p[6]);

    CU_ASSERT_PTR_EQUAL(temp_element_p->network_element_succ, NULL);

    freeNetwork(net_p);

    for (i = 0; i < numero_agenti; i++) freeAgent(agent_p[i]);
    */

}
