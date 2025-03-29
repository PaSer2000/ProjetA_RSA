#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "rsa_common_header.h"

/* ----------- FONCTION DE TESTS PHASE1-----------------*/

void test1(uint8_t tab_a_chiffrer[],uint64_t tab_chiffre[],uint64_t tab_dechiffre[]){
  printf("\n... Lancement du test 1 ...\n\n");
  /*Génération des clés*/
  rsaKey_t publicKey, privateKey;
  genKeysRabin(&publicKey, &privateKey, MAX_PRIME);

  /*Affichage des clés*/
  affichageClefs(&publicKey, &privateKey); 

  /*Tableau initial */
  printf("  Tableau à chiffrer -----> ");
  for (int i=0;  i<4; i++)  {
    printf("%c ", tab_a_chiffrer[i]);
  }
  printf("\n");

  /*Chiffrement du tableau*/
  chiffrementTabOctets(tab_a_chiffrer, tab_chiffre, 5, &publicKey);

  printf("  Tableau chiffré    -----> ");
  for (int i=0;  i<4; i++)  {
     printf("%ld ", tab_chiffre[i]);
  }
  printf("\n");
 
  /*Déchiffrement du tableau*/
  dechiffrementTabOctets(tab_chiffre, tab_dechiffre, 5, &privateKey);

  printf("  Tableau déchiffré  -----> ");
  for (int i=0;  i<4; i++)  {
    printf("%c ", (char)(tab_dechiffre[i]));
  }
  printf("\n\n... Fin du test1 ...\n");
}

void test2(uint8_t tab_a_chiffrer[],uint64_t tab_chiffre[],uint64_t tab_dechiffre[]){
  printf("\n... Lancement du test 2 ...\n\n");

  /*Génération des clés*/
  rsaKey_t publicKey, privateKey;
  genKeysRabin(&publicKey, &privateKey, MAX_PRIME);

  /*Affichage des clés*/
  affichageClefs(&publicKey, &privateKey); 

  /*Tableau initial */
  printf("  Tableau à chiffrer -----> ");
  for (int i=0;  i<20; i++)  {
    printf("%c", tab_a_chiffrer[i]);
  }
  printf("\n");

  /*Chiffrement du tableau*/
  chiffrementTabOctets(tab_a_chiffrer, tab_chiffre, 20, &publicKey);

  printf("  Tableau chiffré    -----> ");
  for (int i=0;  i<20; i++)  {
     printf("%ld ", tab_chiffre[i]);
  }
  printf("\n");
 
  /*Déchiffrement du tableau*/
  dechiffrementTabOctets(tab_chiffre, tab_dechiffre, 20, &privateKey);

  printf("  Tableau déchiffré  -----> ");
  for (int i=0;  i<20; i++)  {
    printf("%c", (char)(tab_dechiffre[i]));
  }
  printf("\n\n... Fin du test2 ...\n");
}

void test3(char* fichier_a_chiffrer){
  printf("\n... Lancement du test 3 ...\n\n");

  /*Génération des clés*/
  rsaKey_t publicKey, privateKey;
  genKeysRabin(&publicKey, &privateKey, MAX_PRIME);

  /*Affichage des clés*/
  affichageClefs(&publicKey, &privateKey); 

  printf("Contenu du fichier à chiffrer:\n");
  system("cat message.txt");
  printf("\n");

  fichier_chiffrement(fichier_a_chiffrer,"message_chiffre.txt",&publicKey);
  printf("Contenu du fichier chiffré:\n");
  system("cat message_chiffre.txt");

  fichier_dechiffrement("message_chiffre.txt","message_dechiffre.txt",&privateKey);
  printf("Contenu du fichier déchiffré:\n");
  system("cat message_dechiffre.txt");
  printf("\n");
  printf("\n... Fin du test3 ... \n");

}

void test_direct(){
  printf("\n... Lancement du test en direct ...\n\n");

  /*Génération des clés*/
  rsaKey_t publicKey, privateKey;
  genKeysRabin(&publicKey, &privateKey, MAX_PRIME);

  /*Affichage des clés*/
  affichageClefs(&publicKey, &privateKey); 

  char buffer[100];
  uint64_t resultat_chiffre[100],resultat_dechiffre[100];
  printf("Saisir un message à chiffrer:\n");
  fgets(buffer, sizeof(buffer), stdin);

  chiffrementTabOctets(buffer,resultat_chiffre,100,&publicKey);
  printf("Message chiffré:");
  for (int i=0;  i<strlen(buffer); i++)  {
    printf("%lu ", resultat_chiffre[i]);
  }

  dechiffrementTabOctets(resultat_chiffre,resultat_dechiffre,100,&privateKey);
  printf("\nMessage déchiffré:");
  for (int i=0;  i<strlen(buffer); i++)  {
    printf("%c", (char)(resultat_dechiffre[i]));
  }

  printf("\n\n... Fin du test en direct ...\n");
}