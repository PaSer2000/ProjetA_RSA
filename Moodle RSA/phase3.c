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
    // utiliser "testP3choix5.txt" pour In et "testP3choix5Out.txt" pour Out
    char commande[50]; // Pour l'utiliser dans la fonction "system" plus bas

    printf("Convertissement de %s en base64 dans %s\n\n", in, out);
    encode_file_base64(in, out);

    printf("Fichier original :\n");
    sprintf(commande, "cat %s", in);
    system(commande);

    printf("\n\nFichier encode en base 64 :\n");
    sprintf(commande, "cat %s", out);
    system(commande);
    printf("\n\n");
}

void convertb64ToBinary(char *in, char *out)
{
    // utiliser "testP3choix5Out.txt" pour In et "testP3choix6Out.txt" pour Out
    char commande[50]; // Pour l'utiliser dans la fonction "system" plus bas

    printf("Convertissement de %s en binaire dans %s\n\n", in, out);
    decode_file_base64(in, out);

    printf("Fichier original :\n");
    sprintf(commande, "cat %s", in);
    system(commande);

    printf("\n\nFichier decode en base 64 :\n");
    sprintf(commande, "cat %s", out);
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
        printf("===== Bienvenue sur l'interprete de commande =====\n");

        fgets(commande, TAILLE_MAX_COMMANDE, stdin);
        sscanf(commande, "%s", choix);

        if (strcmp(choix, "quit") == 0)
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
            printf("https://moodle.univ-tlse3.fr/pluginfile.php/1126789/mod_folder/content/0/interpre%%CC%%80te.pdf?forcedownload=1\n");
        }
        else if(strcmp(choix, "newkeys") == 0){
            genererPairCle();
        }
        else if(strcmp(choix, "bin-2b64") == 0){
            char in[TAILLE_MAX_NOM_FICHIER];
            char out[TAILLE_MAX_NOM_FICHIER];
            sscanf(commande, "%s %s %s", choix, in, out);
            convertBinaryTob64(in, out);
        }
        else if(strcmp(choix, "b64-2bin") == 0){
            char in[TAILLE_MAX_NOM_FICHIER];
            char out[TAILLE_MAX_NOM_FICHIER];
            sscanf(commande, "%s %s %s", choix, in, out);
            convertb64ToBinary(in, out);
        }
        else{
            printf("Entree invalide\n");
        }

    } while (quitter != 'y');
}