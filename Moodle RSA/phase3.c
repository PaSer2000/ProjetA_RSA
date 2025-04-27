/*------------------------------------
    Phase 3 : Interprete de commande
------------------------------------*/

#include "rsa_common_header.h"

void clearBuffer()
{
    // Vidage de la memoire tampon pour clear stdin
    int clean;
    while ((clean = getchar() != '\n') && clean != EOF);
}
void genererPairCle()
{
    printf("Generation des clés publique et privee...\n");
    rsaKey_t publicKey, privateKey;
    genKeysRabin(&publicKey, &privateKey, MAX_PRIME);
    printf("La paire de cle a ete generee :\n");
    affichageClefs(&publicKey, &privateKey);
}

void convertBinaryTob64()
{
    //utiliser "testP3choix5.txt" pour In et testP3choix5Out.txt pour Out
    char *fichierIn = NULL;
    char *fichierOut = NULL;

    printf("Entrez le fichier a chiffrer en base64: ");
    fscanf(stdin, "%s", fichierIn);

    clearBuffer();

    printf("\nEntrez le fichier de destination : ");
    fscanf(stdin, "%s", fichierOut);

    printf("Encodage du fichier en base64...\n");
    encode_file_base64(fichierIn, fichierOut);
    printf("Fichier original :\n");
    
}

int main()
{
    int choix = -1;
    char quitter;
    do // boucle principale tant que l'utiisateur ne souhaite pas quitter l'interprete de commande
    {
        printf("Bienvenue sur le l'interprete de commande,\nquelle action souhaitez vous faire ?\n1. generer des paires de cles et leur associer un identificateur (table des symboles)\n2. chiffrer, dechiffrer avec les cles en utilisant l'identificateur associe\n3. supprimer des cles\n4. lister toutes les clefs\n5. convertir un fichier binaire en base64\n6. convertir un fichier base64 en binaire\n7. sauvegarde des cles dans un fichier\n8. Quitter le terminal\nVotre choix : ");
        do // on boucle tant que le choix n'est pas valide
        {
            fscanf(stdin, "%d", &choix);
            if (choix < 1 || choix > 8)
            {
                printf("Erreur, entrez un nombre valide\nVotre choix : ");
            }
        } while (choix < 1 || choix > 8);

        printf("Vous avez choisi le choix numero %d\n", choix);

        switch (choix)
        {
        case 1:
            genererPairCle();
            break;

        case 2:
            break;

        case 3:
            break;

        case 4:
            break;

        case 5:
            convertBinaryTob64();
            break;

        case 6:
            break;

        case 7:
            break;

        case 8:
            break;
        }

        clearBuffer();

        printf("Voulez vous quitter le terminal ? y/n : ");
        do
        {
            fflush(stdin);
            fscanf(stdin, "%c", &quitter);
            if (quitter != 'y' && quitter != 'n')
            {
                printf("Entrez 'y' ou 'n'\n");
            }

        } while (quitter != 'y' && quitter != 'n');

    } while (quitter != 'y');
}