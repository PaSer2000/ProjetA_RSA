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

void test_intermittent(){
    printf("\n... Lancement du test intermittent ...\n\n");

  /*Génération des clés*/
  rsaKey_t publicKey, privateKey;
  genKeysRabin(&publicKey, &privateKey, MAX_PRIME);

  /*Affichage des clés*/
  affichageClefs(&publicKey, &privateKey); 

  printf("Contenu du fichier à chiffrer:\n");
  system("cat message.txt");
  printf("\n");

  chiffrer_bloc_dans_fichier("message.txt","irresistiblement_chiffre.txt",&publicKey);
  printf("\nContenu du fichier chiffré:\n");


  dechiffrer_bloc_dans_fichier("irresistiblement_chiffre.txt","irresistiblement_vf.txt",&privateKey);
  printf("\n\nContenu du fichier déchiffré:\n");
  system("cat irresistiblement_vf.txt");
  printf("\n");
  
  uint8_t tab[4]={'I','r','r','e'};
  uint32_t res=convert_4byte2int(tab);
  printf("%d\n",res);
  mpz_t resu,bloc;
  mpz_init(resu);
  mpz_init(bloc);
  mpz_set_ui(bloc,res);
  chiffrementBloc(resu,bloc,&publicKey);
  printf("inter**Veleur lue à dechiffre : %d\n",(uint32_t)mpz_get_ui(resu));
  dechiffrementBloc(resu,resu,&privateKey);
  printf("inter**Valeur dechiffree:%d\n",(uint32_t)mpz_get_ui(resu));
  convertInt2uchar((uint32_t)mpz_get_ui(resu),tab);
  for(int i=0;i<4;i++){
    printf("%c",tab[i]);
  }
  printf("\n... Fin du testintermittent ... \n");
}
#endif