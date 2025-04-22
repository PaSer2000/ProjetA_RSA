/// \file rsa_header.h
/// \author Vincent Dugat
/// \date janvier 2025
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <stdint.h> // uintX_t et la suite
#include <gmp.h>

#define BLOCK_SIZE 4 // en octets
#define MAX_PRIME 10000// UINT64_MAX est trop grand sans GMP
// avec GMP on peut gérer de plus grands nombres
#define BLOCK_BASE_64 12 // caractères
#define NAME_MAX_SIZE 32 // caractères

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAXI(a,b) (((a)>(b))?(a):(b))

extern FILE * logfp;

/* Type of a block of data */
typedef uint8_t block_t[BLOCK_SIZE]; // une case par octet

typedef struct tabUint64_s{
 	uint64_t * uint64array;
 	int dim;
} tabUint64_t;

typedef struct rsaKey_s {
 	uint64_t E;// exposant
 	uint64_t N;// modulo
} rsaKey_t;

// struct pour définir une paire de clefs
typedef struct keyPair_s {
 	rsaKey_t pubKey; // (C,N)
 	rsaKey_t privKey; //(U,N)
} keyPair_t;

void genKeysRabin(rsaKey_t *pubKey,rsaKey_t *privKey,uint64_t max_prime);

/*---------------FONCTION PHASE 1--------------*/
void affichageClefs(rsaKey_t *publicKey,rsaKey_t *privateKey);
void chiffrementTabOctets(uint8_t* aChiffrer,uint64_t* result,int taille_aChiffrer,rsaKey_t *publicKey);
void dechiffrementTabOctets(uint64_t* aDechiffrer,uint64_t* result,int taille_aDechiffrer,rsaKey_t *privateKey);
void fichier_chiffrement(char* nomFich,char* nom_fichier_sortie,rsaKey_t *publicKey);
void fichier_dechiffrement(char* nomFich,char* nom_fichier_sortie,rsaKey_t *privateKey);
void conversionFromTabToB64(uint8_t tab[],uint64_t res[],int longueur);
uint64_t puissance_mod_n (uint64_t a, uint64_t e, uint64_t n);

void build_decoding_table();
void base64_cleanup();
char *base64_encode(const unsigned char *data, size_t input_length, size_t *output_length);
unsigned char *base64_decode(const char *data, size_t input_length, size_t *output_length);
void encode_file_base64(const char* input_filename, const char* output_filename);
void decode_file_base64(const char* input_filename, const char* output_filename);

/*---------------FONCTION PHASE 2--------------*/
void puissance_mod_n_gmp(mpz_t res,uint64_t a,uint64_t e,uint64_t n);

#endif
