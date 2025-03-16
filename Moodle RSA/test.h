#ifndef TEST_H
#define TEST

#include <stdint.h>
#include "rsa_common_header.h"

/******FONCTIONS DE TESTS PHASE 1******/
void test1(uint8_t tab_a_chiffrer[],uint64_t tab_chiffre[],uint64_t tab_dechiffre[],rsaKey_t publicKey,rsaKey_t privateKey);
void test2(uint8_t tab_a_chiffrer[],uint64_t tab_chiffre[],uint64_t tab_dechiffre[],rsaKey_t publicKey,rsaKey_t privateKey);


#endif