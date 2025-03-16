/// \file rsa_tools.c
/// \author Vincent Dugat
/// \date summer 2019 update 2025
/// \brief calculs sur les nombres premiers, génération de clefs RSA

#include "rsa_common_header.h"
#include "bezout.h"
#include <stdlib.h>

void erreur(char* msg){
  printf("*** %s ***\n",msg);
  exit(-1);
}

uint64_t random_uint(uint64_t min,uint64_t max){
/// \brief génère un uint64_t aléatoire entre min et max
/// \param[in] min et max des uint64
/// \return n : min≤n≤max
  return (rand()%(max-min)) + min;
}

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

int rabin (uint64_t a, uint64_t n) {
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
} // pgcdFast()

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

/******************PHASE 1.0 - affichage clés************ */
void affichageClefs(rsaKey_t *publicKey,rsaKey_t *privateKey){
  uint64_t moduleChiffrement=publicKey->N; //n=p*q
  uint64_t exposantChiffrementPublic=publicKey->E; //e
  uint64_t exposantChiffrementPrive=privateKey->E;
  printf("\n... Affichage clefs ...\n\nModule de chiffrement: %lu \nExposant de chiffrement publique:%lu \nExposant de chiffrement privé: %lu \n\n... Fin de l'affichage des clefs ...\n\n",moduleChiffrement,exposantChiffrementPublic,exposantChiffrementPrive);
}

/******************PHASE 1.1 - chiffrement et dechiffrement tableau d'octet*************/


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


/******************PHASE 1.2 - chiffrement et dechiffrement fichier*************/

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

/******************PHASE 1.3 - conversion base 64*************/
void conversionFromTabToB64(uint8_t tab[],uint64_t res[],int longueur){
  /*voir fichier otherbase64*/
}

/*Commentaires:
  08/02/2025
	- On peut rajouter dans les fonctions de chiffrement et déchiffrement un 3eme argument "fichier chiffré" et "fichier déchiffré" 
  pour observer le déchiffrement. Ceci pour pouvoir tester le fichier chiffré/déchifré dans le main
  Vous pensez quoi?
  Chaud

*/