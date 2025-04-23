/*-------------MAIN PHASE 2-------------*/


#include "rsa_common_header.h"
#include "test.h"
#include "bezout.h"

int main() {     
  
    mpz_t resultatChiffrement, resultatDechiffrement;
    uint32_t bloc = 450;

    /*Initialisation des variables mpz_t*/
    mpz_init(resultatChiffrement);
    mpz_init(resultatDechiffrement);

    test1Phase2(resultatChiffrement, resultatDechiffrement, bloc);

    /*Libération de la mémoire*/
    mpz_clear(resultatChiffrement);
    mpz_clear(resultatDechiffrement);
  }