#include <stdio.h>
#include <stdint.h>
#include "rsa_common_header.h"


/* ----------- FONCTION DE TESTS PHASE1-----------------*/

void test1(uint8_t tab_a_chiffrer[],uint64_t tab_chiffre[],uint64_t tab_dechiffre[],rsaKey_t publicKey,rsaKey_t privateKey){
  printf("\n... Lancement du test 1 ...\n\n");

  /*Tableau initial */
  printf("  Tableau à chiffrer -----> ");
  for (int i=0;  i<4; i++)  {
    printf(" %c ", tab_a_chiffrer[i]);
  }
  printf("\n");

  /*Chiffrement du tableau*/
  chiffrementTabOctets(tab_a_chiffrer, tab_chiffre, 5, &publicKey);

  printf("  Tableau chiffré    -----> ");
  for (int i=0;  i<4; i++)  {
     printf(" %ld ", tab_chiffre[i]);
  }
  printf("\n");
 
  /*Déchiffrement du tableau*/
  dechiffrementTabOctets(tab_chiffre, tab_dechiffre, 5, &privateKey);

  printf("  Tableau déchiffré  -----> ");
  for (int i=0;  i<4; i++)  {
    printf(" %c ", (char)(tab_dechiffre[i]));
  }
  printf("\n\n... Fin du test1 ...\n");
}

void test2(uint8_t tab_a_chiffrer[],uint64_t tab_chiffre[],uint64_t tab_dechiffre[],rsaKey_t publicKey,rsaKey_t privateKey){
  printf("\n... Lancement du test 2 ...\n\n");

  /*Tableau initial */
  printf("  Tableau à chiffrer -----> ");
  for (int i=0;  i<20; i++)  {
    printf(" %c ", tab_a_chiffrer[i]);
  }
  printf("\n");

  /*Chiffrement du tableau*/
  chiffrementTabOctets(tab_a_chiffrer, tab_chiffre, 20, &publicKey);

  printf("  Tableau chiffré    -----> ");
  for (int i=0;  i<20; i++)  {
     printf(" %ld ", tab_chiffre[i]);
  }
  printf("\n");
 
  /*Déchiffrement du tableau*/
  dechiffrementTabOctets(tab_chiffre, tab_dechiffre, 20, &privateKey);

  printf("  Tableau déchiffré  -----> ");
  for (int i=0;  i<20; i++)  {
    printf(" %c ", (char)(tab_dechiffre[i]));
  }
  printf("\n\n... Fin du test2 ...\n");
}