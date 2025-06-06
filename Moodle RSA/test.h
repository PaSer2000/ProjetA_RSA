#ifndef TEST_H
#define TEST

#include <stdint.h>
#include "rsa_common_header.h"
#include "int2char.h"

/******FONCTIONS DE TESTS PHASE 1******/
void test1(uint8_t tab_a_chiffrer[],uint64_t tab_chiffre[],uint64_t tab_dechiffre[],int longueur);
void test2(uint8_t tab_a_chiffrer[],uint64_t tab_chiffre[],uint64_t tab_dechiffre[],int longueur);
void test3(char* fichier_a_chiffrer);
void test_direct();
void test4();
void test4fichiers();

/******FONCTIONS DE TESTS PHASE 2******/
void test1Phase2(mpz_t resultatChiffrement,mpz_t resultatDechiffrement,uint32_t bloc);
void test1bisPhase2(uint32_t val,uint8_t tab[]);
void test2Phase2(char* fichier_source,char* fichier_dest);
void test3Phase2(char* fichier_cle);
void test4Phase2();
void test5Phase2(char* fichier_cle,char* fichier_a_encoder);

#endif