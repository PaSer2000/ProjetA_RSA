/*-------------MAIN PHASE 1-------------*/


#include "rsa_common_header.h"
#include "test.h"
#include "bezout.h"

int main() {     
  
    /*INITIALISATION POUR TEST TABLEAU OCTETS*/
    uint8_t tab_test_1[5] = {"1234"};
    uint64_t res_test_chiffrement_1[5];
    uint64_t res_test_dechiffrement_1[5];
  
    test1(tab_test_1,res_test_chiffrement_1,res_test_dechiffrement_1);    
  
    uint8_t tab_test_2[20] = {"Avancement du projet"};
    uint64_t res_test_chiffrement_2[20];
    uint64_t res_test_dechiffrement_2[20];
    test2(tab_test_2,res_test_chiffrement_2,res_test_dechiffrement_2);

    /*INITIALISATION POUR TEST FICHIERS*/
    char *fichier_clair = "message.txt";
    test3(fichier_clair);
  
    test_direct();

    //test_base64();
    /*DECOMME?TER QUAND CA FONCTIONNE*/

    encode_file_base64("message.txt", "encoded.txt");
    decode_file_base64("encoded.txt", "decoded.txt");
    
    return 0;
  }