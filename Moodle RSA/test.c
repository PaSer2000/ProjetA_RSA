#include <stdio.h>
#include <stdint.h>
#include "rsa_common_header.h"


/* ----------- FONCTION DE TESTS PHASE1-----------------*/

void test1(uint8_t tab[],uint64_t res[],uint64_t res2[],rsaKey_t publicKey,rsaKey_t privateKey){
  printf("Lancement du test 1 ...\n");

  /*Tableau initial */
  printf("Tableau à chiffrer: ");
  for (int i=0;  i<4; i++)  {
    printf(" %c ", tab[i]);
  }
  printf("\n");

  /*Chiffrement du tableau*/
  chiffrementTabOctets(tab, res, 5, &publicKey);

  printf("Tableau chiffré: ");
  for (int i=0;  i<4; i++)  {
     printf(" %ld ", res[i]);
  }
  printf("\n");
 
  /*Déchiffrement du tableau*/
  dechiffrementTabOctets(res, res2, 5, &privateKey);

  printf("Tableau déchiffré: ");
  for (int i=0;  i<4; i++)  {
    printf(" %c ", itoa(res2[i]));
  }
  printf("\nFin du test1...\n");
}