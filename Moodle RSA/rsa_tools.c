/// \file rsa_tools.c
/// \author Vincent Dugat
/// \date summer 2019 update 2025
/// \brief calculs sur les nombres premiers, génération de clefs RSA

#include <stdlib.h>
#include <gmp.h>
#include "rsa_common_header.h"
#include "bezout.h"
#include "int2char.h"


void erreur(char* msg){
  printf("*** %s ***\n",msg);
  exit(-1);
}

/* PARTIE 3.1.1 */

int premier (uint64_t n) {
  /// \brief test de primarité, crible d'Erathostène
  /// \returns 1 le nombre est premier, 0 sinon
    uint64_t d;

    if (n % 2 == 0)
      return (n == 2);
    for (d = 3; d * d <= n; d = d + 2)
      if (n % d == 0)
        return 0;
    return 1;
}

uint64_t random_uint(uint64_t min,uint64_t max){
  /// \brief génère un uint64_t aléatoire entre min et max
  /// \param[in] min et max des uint64
  /// \return n : min≤n≤max
    return (rand()%(max-min)) + min;
}

int decompose (uint64_t facteur[], uint64_t n){
    /// \brief décompose un entier en facteurs premiers
    /// \param[in] : n l'entier
    /// \pram[out] : facteur = tableau de facteurs
    /// \returns la dimension du tableau de facteurs.
    uint64_t d = 3;
    int i = 0;

    while (n % 2 == 0) {
      facteur[i++] = 2;
      n = n / 2;
    }
    while (d * d <= n)
      if (n % d == 0) {
        facteur[i++] = d;
        n = n / d;
      }
      else
        d = d + 2;
    if (n > 1)
      facteur[i++] = n;
    return i;
}

uint64_t puissance(uint64_t a, uint64_t e) {
    /// \brief puissance russe, une vieille connaissance
    /// \param[in] : a l'entier et e l'exposant
    /// \returns : a^e
    uint64_t p;

    for (p = 1; e > 0; e = e / 2) {
      if (e % 2 != 0)
        p = (p * a);
      a = (a * a);
    }
    return p;
}

uint64_t puissance_mod_n (uint64_t a, uint64_t e, uint64_t n) {
    /// \brief puissance modulaire, calcule a^e mod n
    /// \comment a*a peut dépasser la capacité d'un uint64_t
    /// \comment https://www.labri.fr/perso/betrema/deug/poly/exp-rapide.html
    /// \returns p = a^e mod n
    uint64_t p;
    for (p = 1; e > 0; e = e / 2) {
      if (e % 2 != 0)
        p = (p * a) % n;
      a = (a * a) % n;
    }
    return p;
}

uint64_t genereUint(uint64_t max,int *cpt){
    /// \brief génère un nombre premier inférieur à max avec Eratostène
    /// \param[in] max : la borne sup
    /// \param[out] cpt : nombre d'essais
    /// \returns : le nombre premier
    uint64_t num;
    *cpt=1;
    do{
      num = random_uint(17,max);
    } while (num%2!=1);

    while (!premier(num) && num<max){
      (*cpt)++;
      num=num+2;
    }
    if (num>=max)
      erreur("pb de génération de nombre premier (borne ?)\n");
    return num;
}

int rabin(uint64_t a, uint64_t n) {
    /// \brief test de Rabin sur la pimarité d'un entier
    /// \brief c'est un test statistique
    /// \returns 0 ou 1
    uint64_t p, e, m;
    int i, k;

    e = m = n - 1;
    for (k = 0; e % 2 == 0; k++)
      e = e / 2;

    p = puissance_mod_n (a, e, n);
    if (p == 1) return 1;

    for (i = 0; i < k; i++) {
      if (p == m) return 1;
      if (p == 1) return 0;
      p = (p * p) % n;
    }

    return 0;
}

int64_t genereUintRabin(uint64_t max,int *cpt){
    /// \brief fournit un nombre premier vérifié avec le test de rabin
    /// \returns le nombre premier < max
    uint64_t num;
    *cpt=1;
    int a=2;
    do{
      num = random_uint(17,max);
    } while (num%2!=1);
    while (!rabin(a,num) && num<max){
      (*cpt)++;
      num=num+2;
    }
    return num;
}

uint64_t pgcdFast(uint64_t a,uint64_t b){
    /// \brief pgcd rapide de a et b
    /// \param[in] a et b les deux entiers
    /// \returns pgcd(a,b)
    uint64_t pow2;
    // cas trivial ... PGCD(a,0)=a ou PGCD(0,b)=b
    // dans les deux cas : PGCD(a,b)=a+b si l'un des deux nombres est nul
    if(0==a || b==0) return a+b;
    // on s'occupe de toutes les puissances de deux contenues dans le PGCD(a,b)
    // PGCD( a=2^n.a' , b=2^n.b' )=2^n.PGCD(a',b')
    pow2 = 1u;
    while(!(a&1)) // tant que a est pair
      {
      if(!(b&1)) // si b est de plus pair
        {
        // PGCD( a=2.a' , b=2.b' )=2.PGCD(a',b')
        a     >>= 1;
        b     >>= 1;
        pow2  <<= 1;
        }
      else
        {
        // on rend <a> pair
        do a>>=1;while(!(a&1));
        break; // toutes les puissances de 2 du PGCD(a,b) ont ete traitees
        }
      }

    // on rend <b> pair
    while(!(b&1)) b>>=1;
    // ici les deux nombres sont pairs, le temps de calcul precedent est negligeable
    // devant ce qui suit...
    // information : la difference de deux nombres impairs est pair
    do
      {
      if(a==b)
        {
        break; // bingo ! PGCD(a,b)=a
        }
      else if(a > b)
        {
        a -= b; // PGCD(a,b)=PGCD(a-b,b)
        do a>>=1;while(!(a&1)); // on peut rendre <a> pair car on a plus de puissance de deux dans le PGCD
        }
      else // b > a
        {
        b -= a; // PGCD(a,b)=PGCD(a,b-a)
        do b>>=1;while(!(b&1)); // on peut rendre <b> pair car on a plus de puissance de deux dans le PGCD
        }
      }while(1);

  return pow2*a;
}

void genKeysRabin(rsaKey_t *pubKey,rsaKey_t *privKey,uint64_t max_prime){
    /// \brief génère une paire de "grandes" clefs, adaptées au cryptage RSA par bloc
    /// \param[out] pubKey : clef publique
    /// \param[out] privKey : clef privée

    printf(">>>Max=%lu\n",max_prime);

    int cpt1,cpt2;
    uint64_t num1 = genereUintRabin(max_prime,&cpt1);
    uint64_t num2 = genereUintRabin(max_prime,&cpt2);

    assert(num1 != num2);

    uint64_t N = num1*num2;
    uint64_t M = (num1-1)*(num2-1); //indicatrice d'Euler
    FILE * logfp;
    logfp = fopen("rsa_log.txt", "w");
    if (!logfp) {
      perror("Erreur lors de l'ouverture du fichier de log");
      exit(EXIT_FAILURE);
    }
    fprintf(logfp,"num1 = %lu num2 = %lu cpt1 = %d cpt2 = %d\n",num1,num2,cpt1,cpt2);
    uint64_t C = 2;

    // recherche d'un nombre premier avec M
    while (pgcdFast(C,M)!=1 && C<M){
      C++;
    }

    assert(C<M);// ça serait surprenant que ça échoue

    int64_t U,V;
    bezoutRSA(C,M,&U,&V);
    fprintf(logfp,"Retour de Bézout : U = %lu V = %lu C = %lu M = %lu\n",U,V,C,M);
    fprintf(logfp,"Calcul : C*U + M*V = %lu, C*U + M*V mod M = %lu\n",C*U + M*V,(C*U + M*V)%M);

    assert(2<U && U<M);

    pubKey->E = C;
    pubKey->N = N;
    privKey->E = U;
    privKey->N = N;
}

void inputKey(uint64_t E,uint64_t N,rsaKey_t *key){
    /// \brief pour debug : permet de saisir une clef
    /// \note ça peut servir... ou pas.
    printf("Entrez l'exposant : ");
    scanf("%lu",&E);
    printf("\nEntrez le modulo : ");
    scanf("%lu",&N);
    key->E = E;
    key->N = N;
}

void verifRabin(uint64_t max,int iterations){
    /// \brief vérifie la primarité des nombres premiers générés avec le test de Rabin
    /// \param[in] : le max pour la génération des nombres, le nombre d'itérations du test
    for (int i = 0;i<iterations;i++){
      int cpt = 0;
      uint64_t candidatPremier = genereUintRabin(max,&cpt);
      int res = premier(candidatPremier);
      printf("%lu ",candidatPremier);
      res == 0?printf("non premier\n"):printf("ok\n");
    }
}

/*DEBUT DU CODE ECRIT PAR ETUDIANTS*/

/******************* PARTIE 3.1.2 - PHASE 1.0 - affichage clés ********************* */

void affichageClefs(rsaKey_t *publicKey,rsaKey_t *privateKey){
    uint64_t moduleChiffrement=publicKey->N; //n=p*q
    uint64_t exposantChiffrementPublic=publicKey->E; //e
    uint64_t exposantChiffrementPrive=privateKey->E;
    printf("\n... Affichage clefs ...\n\nModule de chiffrement: %lu \nExposant de chiffrement publique:%lu \nExposant de chiffrement privé: %lu \n\n... Fin de l'affichage des clefs ...\n\n",moduleChiffrement,exposantChiffrementPublic,exposantChiffrementPrive);
}

/******** PARTIE 3.1.3 - PHASE 1.1 - chiffrement et dechiffrement tableau d'octet*******/

void chiffrementTabOctets(uint8_t* aChiffrer,uint64_t* result,int taille_aChiffrer,rsaKey_t *publicKey){
    //public key composé de publicKey->E=exposant public et publicKey->N=produit de p et q

    //identification de n et e pour faciliter la lisibilité
    uint64_t n=publicKey->N;
    uint64_t e=publicKey->E;

    //traduit les éléments un à un
    for(int i=0;i<taille_aChiffrer;i++){
      result[i]=puissance_mod_n(aChiffrer[i],e,n);
    }
}
void dechiffrementTabOctets(uint64_t* aDechiffrer,uint64_t* result,int taille_aDechiffrer,rsaKey_t *privateKey){
    //private key composé de privateKey->E=exposant privé(d) et privateKey->N=n(produit de p et q)

    //identification de n et e pour faciliter la lisibilité
    uint64_t n=privateKey->N;
    uint64_t d=privateKey->E;

    //traduit les éléments un à un
    for(int i=0;i<taille_aDechiffrer;i++){
      result[i]=puissance_mod_n(aDechiffrer[i],d,n);
    }
}


/********** PARTIE 3.1.4 - PHASE 1.2 - chiffrement et dechiffrement fichier********/

void fichier_chiffrement(char* nomFich,char* nom_fichier_sortie,rsaKey_t *publicKey){
    //public key composé de publicKey->E=exposant public et publicKey->N=produit de p et q

    //identification de n et e pour faciliter la lisibilité
    uint64_t n=publicKey->N;
    uint64_t e=publicKey->E;

    //ouverture du fichier en lecture
    FILE* fichAChiffrer=fopen(nomFich,"rt");
    if(fichAChiffrer==NULL){
      perror("Ouverture du fichier à chiffrer:erreur");
      exit(1);
    }

    //ouverture du fichier en écriture
    FILE* fichChiffre=fopen(nom_fichier_sortie,"wt");
    if(fichChiffre==NULL){
      perror("Ouverture fichier chiffrer résultat:erreur");
      exit(2);
    }

    //transformation de chaque caractère et écriture dans le fichier résultat
    char current;
    uint64_t currentChiffrer;
    while ((current = fgetc(fichAChiffrer)) != EOF) {
        // convertir chaque caractère en uint64_t pour renvoyer un uint64_t
        currentChiffrer = puissance_mod_n((uint64_t)current, e, n);
        // ecrire l'entier chiffré dans le fichier de sortie
        fprintf(fichChiffre, "%lu\n", currentChiffrer);
    }

    fclose(fichAChiffrer);
    fclose(fichChiffre);
}

void fichier_dechiffrement(char* nomFich,char* nom_fichier_sortie,rsaKey_t *privateKey){
    //public key composé de privateKey->E=exposant privé et privateKey->N=produit de p et q

    //identification de n et e pour faciliter la lisibilité
    uint64_t n=privateKey->N;
    uint64_t d=privateKey->E;

    //ouverture du fichier en lecture
    FILE* fichADechiffrer=fopen(nomFich,"rt");
    if(fichADechiffrer==NULL){
      perror("Ouverture du fichier à déchiffrer:erreur");
      exit(1);
    }

    //ouverture du fichier en écriture
    FILE* fichDechiffre=fopen(nom_fichier_sortie,"wt");
    if(fichDechiffre==NULL){
      perror("Ouverture fichier déchiffré:erreur");
      exit(2);
    }

    //transformation de chaque caractère et écriture dans le fichier résultat
    uint64_t current, currentDechiffrer;
    while (fscanf(fichADechiffrer, "%lu\n", &current) != EOF) {
        currentDechiffrer = puissance_mod_n(current, d, n);
        // Convertir l'entier déchiffré en caractère ASCII
        fputc((char)currentDechiffrer, fichDechiffre);
    }

    fclose(fichADechiffrer);
    fclose(fichDechiffre);
}

/************ PARTIE 3.1.5 - PHASE 1.3 - conversion base 64 *************/

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
  'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
  'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
  'w', 'x', 'y', 'z', '0', '1', '2', '3',
  '4', '5', '6', '7', '8', '9', '+', '/'}; // 64 caractères

static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};

void build_decoding_table() {
  decoding_table = calloc(256, sizeof(char));
  if (!decoding_table) return;

  for (int i = 0; i < 64; i++)
      decoding_table[(unsigned char) encoding_table[i]] = i;
}

void base64_cleanup() {
  free(decoding_table);
  decoding_table = NULL;
}

char *base64_encode(const unsigned char *data, size_t input_length, size_t *output_length) {
  *output_length = 4 * ((input_length + 2) / 3);
  char *encoded_data = malloc(*output_length + 1);
  if (!encoded_data) return NULL;

  for (size_t i = 0, j = 0; i < input_length;) {
      uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
      uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
      uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;
      uint32_t triple = (octet_a << 16) + (octet_b << 8) + octet_c;

      encoded_data[j++] = encoding_table[(triple >> 18) & 0x3F];
      encoded_data[j++] = encoding_table[(triple >> 12) & 0x3F];
      encoded_data[j++] = encoding_table[(triple >> 6) & 0x3F];
      encoded_data[j++] = encoding_table[triple & 0x3F];
  }

  for (int i = 0; i < mod_table[input_length % 3]; i++)
      encoded_data[*output_length - 1 - i] = '=';

  encoded_data[*output_length] = '\0';
  return encoded_data;
}

unsigned char *base64_decode(const char *data, size_t input_length, size_t *output_length) {
  if (!decoding_table) build_decoding_table();
  if (input_length % 4 != 0) return NULL;

  *output_length = input_length / 4 * 3;
  if (data[input_length - 1] == '=') (*output_length)--;
  if (data[input_length - 2] == '=') (*output_length)--;

  unsigned char *decoded_data = malloc(*output_length);
  if (!decoded_data) return NULL;

  for (int i = 0, j = 0; i+3 < input_length;) {

      uint32_t sextet_a = data[i] == '=' ? 0 : decoding_table[(unsigned char)data[i++]];
      uint32_t sextet_b = data[i] == '=' ? 0 : decoding_table[(unsigned char)data[i++]];
      uint32_t sextet_c = data[i] == '=' ? 0 : decoding_table[(unsigned char)data[i++]];
      uint32_t sextet_d = data[i] == '=' ? 0 : decoding_table[(unsigned char)data[i++]];

      uint32_t triple = (sextet_a << 18) 
      + (sextet_b << 12) 
      + (sextet_c << 6) + sextet_d;

      if (j < *output_length) decoded_data[j++] = (triple >> 16) & 0xFF;
      if (j < *output_length) decoded_data[j++] = (triple >> 8) & 0xFF;
      if (j < *output_length) decoded_data[j++] = triple & 0xFF;
  }
  return decoded_data;
}

void encode_file_base64(const char* input_filename, const char* output_filename) {
  FILE* input = fopen(input_filename, "rb");
  FILE* output = fopen(output_filename, "wb");
  if (!input || !output) {
      if (input) fclose(input);
      if (output) fclose(output);
      fprintf(stderr, "\n --- Erreur ouverture fichiers ---\n\n");
      return;
  }

  uint8_t buffer[3];
  size_t bytes_read;
  size_t output_length;

  while ((bytes_read = fread(buffer, 1, 3, input)) > 0) {
      char* encoded = base64_encode(buffer, bytes_read, &output_length);
      fwrite(encoded, 1, output_length, output);
      free(encoded);
  }

  fclose(input);
  fclose(output);
}

void decode_file_base64(const char* input_filename, const char* output_filename) {
  FILE* input = fopen(input_filename, "r");
  FILE* output = fopen(output_filename, "wb");
  if (!input || !output) {
      if (input) fclose(input);
      if (output) fclose(output);
      return;
  }

  char buffer[4];
  size_t bytes_read;
  size_t output_length;

  while ((bytes_read = fread(buffer, 1, 4, input)) > 0) {
      uint8_t* decoded = base64_decode(buffer, bytes_read, &output_length);
      fwrite(decoded, 1, output_length, output);
      free(decoded);
  }
  fclose(input);
  fclose(output);
}

/************ PARTIE 3.2.1 - PHASE 2  *************/

/* Utile à savoir (source mistral + https://gmplib.org/)
GMP – Principales fonctions et types utiles en cryptographie (C)

Types de base :
---------------
- mpz_t : type pour les entiers de précision arbitraire (grands entiers)
          => utilisé pour stocker les clés RSA, les messages, les résultats...

Initialisation et libération :
------------------------------
- void mpz_init(mpz_t x);
    Initialise une variable mpz_t x à zéro (doit être appelée AVANT toute utilisation) + allocation mémoire.
    
- void mpz_init_set_ui(mpz_t rop, unsigned long int op);
    Initialise rop avec la valeur op (entier non signé classique) + allocation mémoire.
    
- void mpz_clear(mpz_t x);
    Libère la mémoire allouée à x (à faire une fois que t’as fini de l’utiliser).

Affectation :
-------------
- void mpz_set(mpz_t rop, const mpz_t op);
    rop = op
    
- void mpz_set_ui(mpz_t rop, unsigned long int op);
    rop = op (valeur entière classique)

- void mpz_set_str(mpz_t rop, const char *str, int base);
    rop = str (lit un nombre depuis une chaîne en base 10, 16, etc.)

Opérations arithmétiques :
---------------------------
- void mpz_add(mpz_t rop, const mpz_t op1, const mpz_t op2);
    rop = op1 + op2

- void mpz_sub(mpz_t rop, const mpz_t op1, const mpz_t op2);
    rop = op1 - op2

- void mpz_mul(mpz_t rop, const mpz_t op1, const mpz_t op2);
    rop = op1 * op2

- void mpz_powm(mpz_t rop, const mpz_t base, const mpz_t exp, const mpz_t mod);
    rop = base^exp mod mod  ← utilisée en cryptographie (exponentiation modulaire rapide)

- void mpz_mod(mpz_t rop, const mpz_t op1, const mpz_t op2);
    rop = op1 mod op2

- void mpz_gcd(mpz_t rop, const mpz_t op1, const mpz_t op2);
    rop = pgcd(op1, op2)  ← utile pour vérifier que 2 nombres sont premiers entre eux

- void mpz_invert(mpz_t rop, const mpz_t op1, const mpz_t mod);
    rop = inverse de op1 modulo mod (ex : d = e^-1 mod phi(n))

Libération mémoire:
-------------------
- mpz_clear(mpz_t x);
    libère l'espace mémoire associé à x

Comparaison :
-------------
- int mpz_cmp(const mpz_t op1, const mpz_t op2);
    retourne -1 si op1 < op2, 0 si op1 == op2, 1 si op1 > op2

Génération de nombres premiers / aléatoires :
---------------------------------------------
- void mpz_urandomm(mpz_t rop, gmp_randstate_t state, const mpz_t n);
    génère un entier aléatoire uniforme dans [0, n-1]

- int mpz_probab_prime_p(const mpz_t n, int reps);
    retourne si n est probablement premier (test de primalité probabiliste)

Divers :
--------
- gmp_randstate_t : état de générateur aléatoire
- void gmp_randinit_default(gmp_randstate_t state);
    initialise l’état du générateur aléatoire (à utiliser avec mpz_urandomm)
- void gmp_randclear(gmp_randstate_t state);
    libère la mémoire de l’état du générateur

*/

// 3

void puissance_mod_n_gmp(mpz_t res,uint64_t a,uint64_t e,uint64_t n){
  mpz_t base,exposant,modulo;

  //allocation et initalisation des variables utilisées avec GMP
  mpz_init_set_ui(base, a);
  mpz_init_set_ui(exposant, e);
  mpz_init_set_ui(modulo, n);

  //calcul de la puissance modulaire et on stocke dans res
  mpz_powm(res,base,exposant,modulo);

  //libération des variables allouées
  mpz_clear(base);
  mpz_clear(exposant);
  mpz_clear(modulo);
}

// 4a

void chiffrementBloc(mpz_t resultat,uint32_t bloc_a_chiffrer,rsaKey_t *publicKey){
  //public key composé de publicKey->E=exposant public et publicKey->N=produit de p et q

  //identification de n et e pour faciliter la lisibilité
  uint64_t n=publicKey->N;
  uint64_t e=publicKey->E;

  //appel à la fonction
  puissance_mod_n_gmp(resultat,bloc_a_chiffrer,e,n);
}

void dechiffrementBloc(mpz_t resultat,uint32_t bloc_a_dechiffrer,rsaKey_t *privateKey){
  //private key composé de privateKey->E=exposant privé(d) et privateKey->N=n(produit de p et q)

  //identification de n et e pour faciliter la lisibilité
  uint64_t n=privateKey->N;
  uint64_t d=privateKey->E;

  //appel à la fonction
  puissance_mod_n_gmp(resultat,bloc_a_dechiffrer,d,n);
}

// 4b

void chiffrer_bloc_dans_fichier(char* fichier_source,char* fichier_dest,rsaKey_t *publicKey){

  //ouverture des fichiers et controle des erreurs
  FILE* fich_source=fopen(fichier_source,"rb");
  if(fich_source==NULL){
    perror("Ouverture du fichier source : échec\n");
    exit(1);
  }

  FILE* fich_dest=fopen(fichier_dest,"wb");
  if(fich_dest==NULL){
    perror("Ouverture du fichier source : échec\n");
    exit(1);
  }

  //initialisation des variables
  int nbLu;
  mpz_t resultat;
  mpz_init(resultat);
  uint8_t a_convertir[4]={0};
  uint32_t res;

  while ((nbLu=fread(a_convertir,sizeof(uint8_t),4,fich_source)) > 0) {
    //si nbLu plus petit que 4 octets
    if (nbLu<4) {
      for (int i=nbLu; i<4; i++){
        a_convertir[i]=0;
      }
    }
    for(int i=0;i<4;i++){
      printf("%c",a_convertir[i]);
    }
    printf("\n");
    chiffrementBloc(resultat,convert_4byte2int(a_convertir),publicKey);
    //fprintf(fich_dest,"%ld ",mpz_get_ui(resultat));
    res=(uint32_t)mpz_get_ui(resultat);
    printf("res=%d\n",res);
    fwrite(&res,sizeof(uint32_t),1,fich_dest);
  }

  //fermeture des fichiers et libération mémoire du resultat
  fclose(fich_source);
  fclose(fich_dest);
  mpz_clear(resultat);

}

// 4c

void dechiffrer_bloc_dans_fichier(char* fichier_source,char* fichier_dest,rsaKey_t *privateKey){
  //ouverture des fichiers et controle des erreurs
  FILE* fich_source=fopen(fichier_source,"rb");
  if(fich_source==NULL){
    perror("Ouverture du fichier source : échec\n");
    exit(1);
  }

  FILE* fich_dest=fopen(fichier_dest,"wb");
  if(fich_dest==NULL){
    perror("Ouverture du fichier source : échec\n");
    exit(1);
  }

  //initialisation des variables
  uint32_t current;
  mpz_t resultat;
  mpz_init(resultat);
  uint8_t tab4bytes[4]={0};

  while (fread(&current,sizeof(uint32_t),1,fich_source)==1) {
    //déchiffrement du uint32_t qui a été lu
    dechiffrementBloc(resultat,current,privateKey);

    //conversion en 4 caractères
    convertInt2uchar((uint32_t)mpz_get_ui(resultat),tab4bytes);

    //écriture des caractères dans le fichier dest
    for(int i=0;i<4;i++){
      fprintf(fich_dest,"%c",tab4bytes[i]);
    }
  }

  //fermeture des fichiers et libération mémoire du resultat
  fclose(fich_source);
  fclose(fich_dest);
  mpz_clear(resultat);

}

// 4d

void exporterClePublique(char* fichier_out, rsaKey_t* publicKey) {
  FILE* f = fopen(fichier_out, "w");
  if (!f) {
      perror("Erreur export clef");
      exit(1);
  }
  // Export en hex UTF-8
  fprintf(f, "e : %lx\n", publicKey->E);
  fprintf(f, "n : %lx\n", publicKey->N);
  fclose(f);
}

void importerClePublique(char* fichier_avec_cle,unsigned long* exposant,unsigned long* modulo){
  FILE* fich_cle=fopen(fichier_avec_cle,"rb");
  if(fich_cle==NULL){
    perror("Ouverture fichier avec cle : échec\n");
    exit(1);
  }

  //initialisation variables
  char e,n,deuxpoints;

  //on suppose que le fichier est construit de la même manière que la fn exporterClePublique
  //lecture de la première ligne avec l'exposant
  if(fscanf(fich_cle,"%c %c %lx\n",&e,&deuxpoints,exposant)!=3){
    perror("Lecture exposant : erreur\n");
    fclose(fich_cle);
    exit(2);
  }
  if(e!='e' || deuxpoints!=':'){
    perror("Format fichier incorrect (e): erreur\n");
    fclose(fich_cle);
    exit(2);
  }

  //lecture de la deuxieme ligne avec le modulo
  if(fscanf(fich_cle,"%c %c %lx",&n,&deuxpoints,modulo)!=3){
    perror("Lecture modulo : erreur\n");
    fclose(fich_cle);
    exit(2);
  }
  if(n!='n' || deuxpoints!=':'){
    perror("Format fichier incorrect (n): erreur\n");
    fclose(fich_cle);
    exit(2);
  }

  //fermeture fichier
  fclose(fich_cle);

}
