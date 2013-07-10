#ifndef _SUITE_TEST_LIST_OF_AGENT_H
#define	_SUITE_TEST_LIST_OF_AGENT_H

int initTestListOfAgent(void);
int cleanTestListOfAgent(void);
void test_Agent_allocList(void);
void test_Agent_addElementToList_lista_vuota(void);
void test_Agent_addElementToList_di_due_agent_su_lista_vuota(void);
void test_allocNetworkIterator_network_vuota(void);
void test_freeNetworkIterator_network_vuota(void);
void test_allocNetworkIterator_network_un_agent(void);
void test_freeNetworkIterator_network_un_agent(void);
void test_hasNext_network_vuota(void);
void test_hasNext_network_un_agent(void);
void test_next_network_vuota(void);
void test_next_network_un_agent(void);
void test_next_network_due_agent(void);
void test_next_network_tutti_gli_agent(void);
void test_removeLastReturned_network_vuota(void);
void test_removeLastReturned_network_un_agent(void);
void test_removeLastReturned_primo_di_due_agent(void);
void test_removeLastReturned_secondo_di_due_agent(void);
void test_removeLastReturned_primo_di_molti_agent(void);
void test_removeLastReturned_ultimo_di_molti_agent(void);
void test_removeLastReturned_in_mezzo_a_molti_agent(void);
void test_removeAgent_network_vuota(void);
void test_removeAgent_network_un_agent_differente(void);
void test_removeAgent_network_un_agent_uguale(void);
void test_removeAgent_network_molti_agent_nessuno_uguale(void);
void test_removeAgent_network_molti_agent_uno_uguale(void);
void test_removeAgent_network_molti_agent_uno_ripetuto(void);

#endif	/* _SUITE_TEST_LIST_OF_AGENT_H */

