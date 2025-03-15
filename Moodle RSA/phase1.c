/*-------------MAIN PHASE 1-------------*/


#include "rsa_common_header.h"
#include "test.h"
#include "bezout.h"


int main() {

    /*Génération des clés*/
    rsaKey_t publicKey, privateKey;
    genKeysRabin(&publicKey, &privateKey, MAX_PRIME);
  
    /*Affichage des clés*/
    printf("Clés générées :\n");
    affichageClefs(&publicKey, &privateKey);  
  
    /*INITIALISATION POUR TEST TABLEAU OCTETS*/
    uint8_t tab[5] = {"1234"};
    uint64_t res[5];
    uint64_t res2[5];
  
    test1(tab,res,res2,publicKey,privateKey);
      
    /*test perso*/
    printf("\n\n *****Test perso******\n\n");
    printf("caractere 'a' après l'avoir mis à la puissance de la clé publique = %d\n",puissance_mod_n('a',publicKey.E,publicKey.N));
    printf("valeur 912673(=caractère 'a' chiffré) dechiffré = %d\n",puissance_mod_n(912673,privateKey.E,publicKey.N));
    printf("sah y a un pb\n");
    
  
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