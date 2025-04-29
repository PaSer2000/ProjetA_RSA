/*-------------MAIN PHASE 2-------------*/


#include "rsa_common_header.h"
#include "test.h"
#include "bezout.h"

int main() {     
    /*INITIALISATION DES VARIABLES POUR LES TESTS DE BLOCS*/
    mpz_t resultatChiffrement, resultatDechiffrement;
    uint32_t bloc = 3477790;
    mpz_init(resultatChiffrement);
    mpz_init(resultatDechiffrement);

    test1Phase2(resultatChiffrement, resultatDechiffrement, bloc);

    /*LIBERATION MEMOIRE*/
    mpz_clear(resultatChiffrement);
    mpz_clear(resultatDechiffrement);

    uint8_t tab[4];
    test1bisPhase2(343434,tab);

    /*INITIALISATION TEST FICHIER*/
    test2Phase2("irresistiblement.txt","irresistiblement_chiffre.txt");

    test3Phase2("cle_chiffree_hexa.txt");

    test4Phase2();

    test5Phase2("cle_chiffree_hexa.txt","message_Test_Phase2.txt");

    test_intermittent();

  }