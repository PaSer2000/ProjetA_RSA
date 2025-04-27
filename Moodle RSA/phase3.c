/*------------------------------------
    Phase 3 : Interprete de commande
------------------------------------*/

#include "rsa_common_header.h"
#include <string.h>
#define TAILLE_MAX_NOM_FICHIER 30
#define TAILLE_MAX_COMMANDE 100

void clearBuffer()
{
    // Vidage de la memoire tampon pour clear stdin
    int clean;
    while ((clean = getchar() != '\n') && clean != EOF)
        ;
}

void genererPairCle()
{
    printf("Generation des clés publique et privee...\n");
    rsaKey_t publicKey, privateKey;
    genKeysRabin(&publicKey, &privateKey, MAX_PRIME);
    printf("La paire de cle a ete generee :\n");
    affichageClefs(&publicKey, &privateKey);
}

void convertBinaryTob64(char* in, char *out)
{
    printf("Les fichiers recup sont : %s et %s\n", in, out);
    // utiliser "testP3choix5.txt" pour In et "testP3choix5Out.txt" pour Out
    char fichierIn[TAILLE_MAX_NOM_FICHIER];
    char fichierOut[TAILLE_MAX_NOM_FICHIER];
    char commande[50]; // Pour l'utiliser dans la fonction "system" plus bas

    fscanf(stdin, "%s", fichierIn);
    clearBuffer();

    fscanf(stdin, "%s", fichierOut);
    clearBuffer();

    printf("Convertissement de %s en base64 dans %s\n\n", fichierIn, fichierOut);
    encode_file_base64(fichierIn, fichierOut);

    printf("Fichier original :\n");
    sprintf(commande, "cat %s", fichierIn);
    system(commande);

    printf("\n\nFichier encode en base 64 :\n");
    sprintf(commande, "cat %s", fichierOut);
    system(commande);
    printf("\n\n");
}

void convertb64ToBinary()
{
    // utiliser "testP3choix5Out.txt" pour In et "testP3choix6Out.txt" pour Out
    char fichierIn[TAILLE_MAX_NOM_FICHIER];
    char fichierOut[TAILLE_MAX_NOM_FICHIER];
    char commande[50]; // Pour l'utiliser dans la fonction "system" plus bas

    fscanf(stdin, "%s", fichierIn);
    clearBuffer();

    fscanf(stdin, "%s", fichierOut);
    clearBuffer();

    printf("Convertissement de %s en binaire dans %s\n\n", fichierIn, fichierOut);
    decode_file_base64(fichierIn, fichierOut);

    printf("Fichier original :\n");
    sprintf(commande, "cat %s", fichierIn);
    system(commande);

    printf("\n\nFichier decode en base 64 :\n");
    sprintf(commande, "cat %s", fichierOut);
    system(commande);
    printf("\n\n");
}

int main()
{
    char commande[TAILLE_MAX_COMMANDE];
    char choix[50];
    char quitter;
    do // boucle principale tant que l'utiisateur ne souhaite pas quitter l'interprete de commande
    {
        printf("Bienvenue sur l'interprete de commande\n");

        fscanf(stdin, "%s", commande);
        sscanf(commande, "%s", choix);
        printf("Le choix est : %s", choix);
        //clearBuffer();

        if (strcmp(choix, "leave") == 0)
        {
            printf("Voulez vous quitter le terminal ? y/n : ");
            do
            {
                fscanf(stdin, "%c", &quitter);
                clearBuffer();
                if (quitter != 'y' && quitter != 'n')
                {
                    printf("Entrez 'y' ou 'n'\n");
                }

            } while (quitter != 'y' && quitter != 'n');
        }
        else if(strcmp(choix, "help") == 0){
            printf("1. generer des paires de cles et leur associer un identificateur (table des symboles)\n2. chiffrer, dechiffrer avec les cles en utilisant l'identificateur associe\n3. supprimer des cles\n4. lister toutes les clefs\n5. convertir un fichier binaire en base64\n6. convertir un fichier base64 en binaire\n7. sauvegarde des cles dans un fichier\n8. Quitter le terminal\n");
        }
        else if(strcmp(choix, "newkeys") == 0){
            genererPairCle();
        }
        else if(strcmp(choix, "bin-2b64") == 0){
            char in[TAILLE_MAX_NOM_FICHIER];
            char out[TAILLE_MAX_NOM_FICHIER];
            sscanf(commande, "%s", in);
            sscanf(commande, "%s", out);
            convertBinaryTob64(in, out);
        }
        else if(strcmp(choix, "b64-2bin") == 0){
            convertb64ToBinary();
        }
        else{
            printf("Entree invalide\n");
        }

    } while (quitter != 'y');
}