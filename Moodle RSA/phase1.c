/*-------------MAIN PHASE 1-------------*/


#include "rsa_common_header.h"
#include "test.h"
#include "bezout.h"


int main() {

    /*Génération des clés*/
    rsaKey_t publicKey, privateKey;
    genKeysRabin(&publicKey, &privateKey, MAX_PRIME);
  
    /*Affichage des clés*/
    affichageClefs(&publicKey, &privateKey);  
  
    /*INITIALISATION POUR TEST TABLEAU OCTETS*/
    uint8_t tab_test_1[5] = {"1234"};
    uint64_t res_test_chiffrement_1[5];
    uint64_t res_test_dechiffrement_1[5];
  
    test1(tab_test_1,res_test_chiffrement_1,res_test_dechiffrement_1,publicKey,privateKey);    
  
    uint8_t tab_test_2[20] = {"Avancement du projet"};
    uint64_t res_test_chiffrement_2[20];
    uint64_t res_test_dechiffrement_2[20];
    test2(tab_test_2,res_test_chiffrement_2,res_test_dechiffrement_2,publicKey,privateKey);

    /*INITIALISATION POUR TEST FICHIERS*/
    char *fichier_clair = "message.txt";
    //char *fichier_chiffre = "message_chiffre.txt";
    //char *fichier_dechiffre = "message_dechiffre.txt";
    // Chiffrement
    char* fichierChiffre = chiffrementFichier(fichier_clair, &publicKey);
    //printf("Message chiffré dans %s\n", fichier_chiffre);
  
    // Déchiffrement
    dechiffrementFichier(fichierChiffre, &privateKey);
    //printf("Message déchiffré dans %s\n", fichier_dechiffre);
  
    return 0;
  }