#ifndef _SUITE_TEST_BUFFER_H
#define	_SUITE_TEST_BUFFER_H

int initTestBuffer(void);
int cleanTestBuffer(void);
void test_put_non_bloccante_buffer_vuoto(void);
void test_put_bloccante_buffer_vuoto(void);
void test_get_non_bloccante_buffer_pieno(void);
void test_get_bloccante_buffer_pieno(void);
void test_put_non_bloccante_buffer_pieno(void);
void test_put_bloccante_buffer_pieno(void);
void test_get_non_bloccante_buffer_vuoto(void);
void test_get_bloccante_buffer_vuoto(void);
void test_prima_un_consumatore_poi_un_produttore_concorrenti(void);
void test_prima_un_produttore_poi_un_consumatore_concorrenti(void);
void test_molti_produttori_su_buffer_inizialmente_vuoto(void);
void test_molti_consumatori_su_buffer_inizialmente_pieno(void);

#endif	/* _SUITE_TEST_BUFFER_H */

