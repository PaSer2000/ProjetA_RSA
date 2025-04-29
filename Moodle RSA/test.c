#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "rsa_common_header.h"
#include "int2char.h"


/* ------------------------ FONCTION DE TESTS PHASE1------------------------- */

/******************* PARTIE 3.1.2 - PHASE 1.0 - affichage clés ********************* */

void test1(uint8_t tab_a_chiffrer[],uint64_t tab_chiffre[],uint64_t tab_dechiffre[],int longueur){
  printf("\n... Lancement du test 1 ...\n\n");
  /*Génération des clés*/
  rsaKey_t publicKey, privateKey;
  genKeysRabin(&publicKey, &privateKey, MAX_PRIME);

  /*Affichage des clés*/
  affichageClefs(&publicKey, &privateKey); 

  /*Tableau initial */
  printf("  Tableau à chiffrer -----> ");
  for (int i=0;  i<longueur; i++)  {
    printf("%c ", tab_a_chiffrer[i]);
  }
  printf("\n");

  /*Chiffrement du tableau*/
  chiffrementTabOctets(tab_a_chiffrer, tab_chiffre, longueur, &publicKey);

  printf("  Tableau chiffré    -----> ");
  for (int i=0;  i<longueur; i++)  {
     printf("%ld ", tab_chiffre[i]);
  }
  printf("\n");
 
  /*Déchiffrement du tableau*/
  dechiffrementTabOctets(tab_chiffre, tab_dechiffre, 5, &privateKey);

  printf("  Tableau déchiffré  -----> ");
  for (int i=0;  i<longueur; i++)  {
    printf("%c ", (char)(tab_dechiffre[i]));
  }
  printf("\n\n... Fin du test1 ...\n");
}



/******** PARTIE 3.1.3 - PHASE 1.1 - chiffrement et dechiffrement tableau d'octet*******/

void test2(uint8_t tab_a_chiffrer[],uint64_t tab_chiffre[],uint64_t tab_dechiffre[],int longueur){
  printf("\n... Lancement du test 2 ...\n\n");

  /*Génération des clés*/
  rsaKey_t publicKey, privateKey;
  genKeysRabin(&publicKey, &privateKey, MAX_PRIME);

  /*Affichage des clés*/
  affichageClefs(&publicKey, &privateKey); 

  /*Tableau initial */
  printf("  Tableau à chiffrer -----> ");
  for (int i=0;  i<longueur; i++)  {
    printf("%c", tab_a_chiffrer[i]);
  }
  printf("\n");

  /*Chiffrement du tableau*/
  chiffrementTabOctets(tab_a_chiffrer, tab_chiffre, longueur, &publicKey);

  printf("  Tableau chiffré    -----> ");
  for (int i=0;  i<20; i++)  {
     printf("%ld ", tab_chiffre[i]);
  }
  printf("\n");
 
  /*Déchiffrement du tableau*/
  dechiffrementTabOctets(tab_chiffre, tab_dechiffre, longueur, &privateKey);

  printf("  Tableau déchiffré  -----> ");
  for (int i=0;  i<longueur; i++)  {
    printf("%c", (char)(tab_dechiffre[i]));
  }
  printf("\n\n... Fin du test2 ...\n");
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
  size_t taille_reelle = strlen(buffer);

  chiffrementTabOctets((uint8_t*)buffer,resultat_chiffre,100,&publicKey);

  printf("Message chiffré:");
  for (int i=0;  i<taille_reelle+1; i++)  {
    printf("%lu ", resultat_chiffre[i]);
  }

  dechiffrementTabOctets(resultat_chiffre,resultat_dechiffre,100,&privateKey);
  printf("\nMessage déchiffré:");
  for (int i=0;  i<taille_reelle+1; i++)  {
    printf("%c", (char)(resultat_dechiffre[i]));
  }

  printf("\n\n... Fin du test en direct ...\n");
}



/********** PARTIE 3.1.4 - PHASE 1.2 - chiffrement et dechiffrement fichier********/  

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



/************ PARTIE 3.1.5 - PHASE 1.3 - conversion base 64 *************/
 
void test4() {

  printf("\n... Lancement du test 4 (base64) ...\n\n");

  // Test d'encodage
  const char *message = "Hello, World!";

  size_t encoded_length;
  char *encoded = base64_encode((const unsigned char *)message, strlen(message), &encoded_length);
  if (encoded) {
      printf("Encodage Base64 : %s\n", encoded);
  } else {
      printf("Échec de l'encodage Base64\n");
  }

  // Test de décodage
  size_t decoded_length; 
  unsigned char *decoded = base64_decode(encoded, encoded_length, &decoded_length);
  if (decoded) {
      printf("Décodage Base64 : %.*s\n", (int)decoded_length, decoded);
  } else {
      printf("Échec du décodage Base64\n");
  }
   
  free(encoded);
  free(decoded);
  base64_cleanup();

  printf("\n... Fin du test4 (base64) ... \n");
}

void test4fichiers() {

  printf("\n... Lancement du test 4 avec fichiers (base64) ...\n\n");

  printf("Contenu du fichier à chiffrer:\n");
  system("cat message.txt");
  printf("\n");

  encode_file_base64("message.txt", "encoded.txt");
  printf("Contenu du fichier chiffré:\n");
  system("cat encoded.txt");
  printf("\n");

  decode_file_base64("encoded.txt", "decoded.txt");
  printf("Contenu du fichier déchiffré:\n");
  system("cat decoded.txt");
  printf("\n");


  printf("\n... Fin du test4 avec fichiers (base64) ... \n");

}

/* ------------------------ FONCTION DE TESTS PHASE2------------------------- */

/************ PARTIE 3.2.1 - PHASE 2  *************/
void test1Phase2(mpz_t resultatChiffrement,mpz_t resultatDechiffrement,uint32_t bloc){
  printf("\n... Lancement du test 1 ...\n\n");

  /*Génération des clés*/
  rsaKey_t publicKey, privateKey;
  genKeysRabin(&publicKey, &privateKey, MAX_PRIME);

  /*Affichage des clés*/
  affichageClefs(&publicKey, &privateKey); 

  /*Tableau initial */
  printf("  Bloc à chiffrer ----> %d\n",bloc);

  // conversion de bloc (uint32_t) en mpz_t
  mpz_t bloc_mpz;
  mpz_init_set_ui(bloc_mpz, bloc);

  /*Chiffrement du tableau*/
  chiffrementBloc(resultatChiffrement,bloc_mpz,&publicKey);
  gmp_printf("  Bloc chiffré    ----> %Zd\n", resultatChiffrement); 

  /*Déchiffrement du tableau*/
  dechiffrementBloc(resultatDechiffrement,resultatChiffrement,&privateKey);
  gmp_printf("  Bloc déchiffré  ----> %Zd\n", resultatDechiffrement);  

  printf("\n... Fin du test1 ...\n");
}

void test1bisPhase2(uint32_t val,uint8_t tab[]) {
  printf("\n... Lancement du test 1 bis ...\n\n");
  /*test conversion basique avec un élément*/
  printf("Valeur initiale : %d\nConversion en tab de char...\n",val);
  convertInt2uchar(val,tab);
  printf("Reconversion en entier...\n");
  printf("Valeur finale : %d\n\n",convert_4byte2int(tab));

  /*test avec un tableau de valeurs*/
  uint32_t tab_valeurs[5]={340,787,32,8888,0};

  printf("Contenu du tableau :");
  for(int i=0;i<5;i++) {
    printf(" %d",tab_valeurs[i]);
  }
  printf("\n");

  uint8_t en_uint8[4];  // Tableau de 4 octets pour chaque conversion
  uint32_t retour;
  for(int i=0;i<5;i++) {
    printf("Conversion en tab de uint8_t de la valeur n°%d...\n",i);
    convertInt2uchar(tab_valeurs[i],en_uint8);  // Passe un tableau de 4 octets
    printf("Conversion de tab de uint8_t en uint32 de la valeur n°%d...\n",i);
    retour=convert_4byte2int(en_uint8);  // Passe l'adresse du tableau
    printf("Valeur finale retrouvée : %d\n",retour);
  }

  printf("\nTestons maintenant dans le sens inverse...\n");
  /*init tableau*/
  uint8_t tab_car[]={'p','r','o','j'};
  printf("Caractères initiaux : ");
  for(int i=0;i<4;i++){
    printf("%c",tab_car[i]);
  }
  printf("\n");
  printf("Conversion des caractères en int ...\n");
  uint32_t res_en_int=convert_4byte2int(tab_car);

  printf("Conversion de l'int en caractères ...\nResultat final: ");
  uint8_t res_final[4];
  convertInt2uchar(res_en_int,res_final);

  for(int i=0;i<4;i++){
    printf("%c",tab_car[i]);
  }
  printf("\n\n... Fin du test 1bis ...\n");
}

void test2Phase2(char* fichier_source,char* fichier_dest){
  printf("\n... Lancement du test 2 ...\n\n");

  /*Génération des clés*/
  rsaKey_t publicKey, privateKey;
  genKeysRabin(&publicKey, &privateKey, MAX_PRIME);

  /*Affichage des clés*/
  affichageClefs(&publicKey, &privateKey); 

  printf("Contenu du fichier à chiffrer:\n");
  system("cat irresistiblement.txt");
  printf("\n");

  chiffrer_bloc_dans_fichier(fichier_source,fichier_dest,&publicKey);
  printf("\nContenu du fichier chiffré:\n");
  system("cat irresistiblement_chiffre.txt");

  dechiffrer_bloc_dans_fichier("irresistiblement_chiffre.txt","irresistiblement_vf.txt",&privateKey);
  printf("\n\nContenu du fichier déchiffré:\n");
  system("cat irresistiblement_vf.txt");
  printf("\n");
  
  printf("\n... Fin du test2 ... \n");
}


void test3Phase2(char* fichier_cle){
  printf("\n... Lancement du test 3 ...\n\n");

  /*Génération des clés*/
  rsaKey_t publicKey, privateKey;
  genKeysRabin(&publicKey, &privateKey, MAX_PRIME);

  /*Affichage des clés*/
  affichageClefs(&publicKey, &privateKey); 

  /*chiffrement de la clé en hexa dans un fichier*/
  exporterClePublique(fichier_cle,&publicKey);
  printf("Ecriture des clés en hexa dans le fichier...\n");
  system("cat cle_chiffree_hexa.txt");
  printf("\n");

  /*lecture de la cle en hexa dans le fichier*/
  unsigned long exposant,modulo;
  importerClePublique(fichier_cle,&exposant,&modulo);
  printf("Lecture des clés dans le fichier...\n");
  printf("Valeurs initiales -> e : %ld et n : %ld\n",publicKey.E,publicKey.N);
  printf("Valeurs lues      -> e : %ld et n : %ld\n",exposant,modulo);
  if(publicKey.E==exposant && publicKey.N==modulo){
    printf("Test réussi : TRUE\n");
  }else{
    printf("Test réussi : FALSE\n");
  }
  printf("\n... Fin du test3 ... \n");
}

void test4Phase2() {

  printf("\n... Lancement du test 4 avec fichiers (base64) ...\n\n");

  printf("Contenu du fichier à chiffrer:\n");
  system("cat message.txt");
  printf("\n");

  encode_file_base64("message.txt", "encoded.txt");
  printf("\nContenu du fichier chiffré:\n");
  system("cat encoded.txt");
  printf("\n");

  decode_file_base64("encoded.txt", "decoded.txt");
  printf("\nContenu du fichier déchiffré:\n");
  system("cat decoded.txt");
  printf("\n");


  printf("\n... Fin du test4 avec fichiers (base64) ... \n");

}

void test5Phase2(char* fichier_cle,char* fichier_a_encoder){
  printf("\n... Lancement du test 5 ...\n\n");

  printf("*****Actions effectuées côté expéditeur de clé*****\n\n");
  /*Génération des clés*/
  rsaKey_t publicKey, privateKey;
  genKeysRabin(&publicKey, &privateKey, MAX_PRIME);

  /*Affichage des clés*/
  affichageClefs(&publicKey, &privateKey); 

  /*Export de la clé*/
  printf("Ecriture de la clé publique en hexa dans un fichier...\n");
  exporterClePublique(fichier_cle,&publicKey);
  printf("*****Fin des actions de l'expéditeur de clé, début des actions de l'émetteur de message*****\n\n");

  /*Récupération de la clé*/
  printf("Le fichier %s contient la clé publique reçu en hexadécimal.\nContenu:\n",fichier_cle);
  system("cat cle_chiffree_hexa.txt");
  printf("\nOn exporte les clés ...\n\n");
  unsigned long exposant,modulo;
  importerClePublique(fichier_cle,&exposant,&modulo);
  rsaKey_t cleLu;
  cleLu.E=exposant;
  cleLu.N=modulo;

  /*Chiffrement du message*/
  printf("Le fichier %s contient le message que l'on veut transmettre.\nContenu:\n",fichier_a_encoder);
  system("cat message_Test_Phase2.txt");

  printf("\n\nChiffrement du fichier...\n");
  chiffrer_bloc_dans_fichier(fichier_a_encoder,"encoded_Test5.txt",&cleLu);
  printf("Contenu du fichier chiffré:\n");
  system("cat encoded_Test5.txt");

  /*Conversion du message en base 64*/
  printf("\nConversion en base64...\n");
  encode_file_base64("encoded_Test5.txt", "encoded_test_b64.txt");
  printf("Contenu du fichier en base64:\n");
  system("cat encoded_test_b64.txt");

  
  printf("\n\n*****Actions effectuées côté récepteur de message*****\n");
  printf("Le fichier encoded_test_b64.txt contient le message chiffré en base 64.\nConversion de base64 en binaire...\n");
  printf("Contenu du fichier chiffré en binaire:\n");

  /*Conversion de base 64 en binaire*/
  decode_file_base64("encoded_test_b64.txt","decoded_test5.txt");
  system("cat decoded_test5.txt");
  
  //printf("Dechiffrement du fichier...\nContenu du message totalement dechiffré:\n");
  /*Conversion de binaire en texte*/ 
  /*dechiffrer_bloc_dans_fichier("decoded_test6.txt","final_message.txt",&privateKey);
  system("cat final_message.txt");
  */
  
  printf("\n... Fin du test5 ... \n");
}


