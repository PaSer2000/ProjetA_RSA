/*------------------------------------
    Phase 3 : Interprete de commande
------------------------------------*/

#include "rsa_common_header.h"
#include <string.h>
#include <unistd.h>
#define TAILLE_MAX_NOM_FICHIER 30
#define TAILLE_MAX_COMMANDE 100

//------------------------------------Structure dynamique pour stocker les clés---------------------------------------
typedef struct
{
    rsaKey_t public;
    rsaKey_t private;
    int id;
} keyIdentifier;

typedef struct s_node
{
    struct s_node *next;
    struct s_node *previous;
    keyIdentifier *keyStruct;
} node;

typedef struct
{
    node *sentinel;
    int size;
} keysDynamicList;

int noDouble(keysDynamicList *list, int id); // besoin car une fonction créee avant elle utilise noDouble

// Fonction pour créer la liste dynamique de clés
keysDynamicList *list_create(void)
{
    keysDynamicList *l = malloc(sizeof(keysDynamicList));
    l->size = 0;
    l->sentinel = malloc(sizeof(node));
    l->sentinel->next = l->sentinel;
    l->sentinel->previous = l->sentinel;
    l->sentinel->keyStruct = NULL;
    return l;
}

// Fonction pour supprimer la liste dynamique de clés
void list_delete(keysDynamicList **l)
{
    node *element;
    while ((*l)->sentinel->next != (*l)->sentinel)
    {
        element = (*l)->sentinel->next;
        (*l)->sentinel->next = element->next;
        free(element->keyStruct);
        free(element);
        (*l)->size -= 1;
    }
    free((*l)->sentinel);
    free(*l);
    *l = NULL;
}

// Fonction pour créer une nouvelle clé
keyIdentifier *createNewKeys(rsaKey_t public, rsaKey_t private, int id)
{
    keyIdentifier *new = malloc(sizeof(keyIdentifier));
    new->id = id;
    new->private = private;
    new->public = public;
    return new;
}

// Ajouter une clé à la liste
keysDynamicList *list_push_back(keysDynamicList *l, keyIdentifier *keys)
{
    node *newElement = malloc(sizeof(node));
    newElement->keyStruct = keys;
    l->sentinel->previous->next = newElement;
    newElement->next = l->sentinel;
    newElement->previous = l->sentinel->previous;
    l->sentinel->previous = newElement;
    l->size += 1;
    return l;
}

// Lister toutes les clés
keysDynamicList *list_map(keysDynamicList *l)
{
    node *element = l->sentinel->next;
    while (element != l->sentinel)
    {
        printf("\n----------\nPaire n° %d : ", element->keyStruct->id);
        affichageClefs(&(element->keyStruct->public), &(element->keyStruct->private));
        printf("\n");
        element = element->next;
    }
    return l;
}

// Supprimer une clé par son identifiant
keysDynamicList *supprimerCle(keysDynamicList *l, int id)
{
    node *element = l->sentinel->next;
    while (element != l->sentinel)
    {
        if (element->keyStruct->id == id)
        {
            element->previous->next = element->next;
            element->next->previous = element->previous;
            printf("Suppression de la paire n°%d\n", element->keyStruct->id);
            free(element->keyStruct);
            free(element);
            l->size -= 1;
            return l;
        }
        element = element->next;
    }
    printf("Clé avec identifiant %d non trouvée.\n", id);
    return l;
}

keyIdentifier *getKeyWithID(keysDynamicList *list, int id)
{
    if (noDouble(list, id) == 1)
    { // Utilisation dérivée de la fonction noDouble
        node *key = list->sentinel->next;
        for (; key != list->sentinel; key = key->next)
        {
            if (key->keyStruct->id == id)
            {
                return key->keyStruct;
            }
        }
    }
    printf("Impossible d'obtenir les cles\n");
    return NULL;
}

//--------------------------------------------------------------------------------------------------------------------

// Fonction de sauvegarde des clés dans un fichier sécurisé
void sauvegarderCle(keysDynamicList *keyList, const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }
    fclose(file);
}

// Fonction de nettoyage du tampon d'entrée
void clearBuffer()
{
    int clean;
    while ((clean = getchar() != '\n') && clean != EOF)
        ;
}
void genererPairCle(keysDynamicList *keyList, int id)
{
    printf("Generation des clés publique et privee...\n");
    rsaKey_t publicKey, privateKey;
    genKeysRabin(&publicKey, &privateKey, MAX_PRIME);
    printf("La paire de cle n°%d a ete generee :\n", id);
    affichageClefs(&publicKey, &privateKey);
    list_push_back(keyList, createNewKeys(publicKey, privateKey, id));
}

int noDouble(keysDynamicList *list, int id)
{
    node *key = list->sentinel->next;
    for (; key != list->sentinel; key = key->next)
    {
        if (key->keyStruct->id == id)
        {
            return 1;
        }
    }
    return 0;
}

void listAllKeys(keysDynamicList *keyList)
{
    if (keyList->size > 0)
    {
        list_map(keyList);
        return;
    }
    printf("Aucune paire de cle stockees\n");
}

void convertBinaryTob64(char *in, char *out)
{
    // utiliser "P3test1.txt" pour In et "P3test2.txt" pour Out
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
    // utiliser "P3test2.txt" pour In et "P3test3.txt" pour Out
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
    keysDynamicList *mainKeyList = list_create();
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
        else if (strcmp(choix, "help") == 0)
        {
            printf("https://moodle.univ-tlse3.fr/pluginfile.php/1126789/mod_folder/content/0/interpre%%CC%%80te.pdf?forcedownload=1\n");
        }
        else if (strcmp(choix, "newkeys") == 0)
        {
            int id;
            sscanf(commande, "%s %d", choix, &id);
            if (noDouble(mainKeyList, id) == 0)
            { // On vérifie qu'il n'y ai pas deja une cle avec cet id, pas de double
                genererPairCle(mainKeyList, id);
            }
            else
            {
                printf("Error : Une paire de cle porte deja cet identifiant\n");
            }
        }
        else if (strcmp(choix, "bin-2b64") == 0)
        {
            char in[TAILLE_MAX_NOM_FICHIER];
            char out[TAILLE_MAX_NOM_FICHIER];
            sscanf(commande, "%s %s %s", choix, in, out);
            convertBinaryTob64(in, out);
        }
        else if (strcmp(choix, "b64-2bin") == 0)
        {
            char in[TAILLE_MAX_NOM_FICHIER];
            char out[TAILLE_MAX_NOM_FICHIER];
            sscanf(commande, "%s %s %s", choix, in, out);
            convertb64ToBinary(in, out);
        }
        else if (strcmp(choix, "rmkeys") == 0)
        {
            int idCmp;
            sscanf(commande, "%s %d", choix, &idCmp);
            mainKeyList = supprimerCle(mainKeyList, idCmp);
        }
        else if (strcmp(choix, "save") == 0)
        {
            sauvegarderCle(mainKeyList, "clé.txt");
        }
        else if (strcmp(choix, "listkeys") == 0)
        {
            listAllKeys(mainKeyList);
        }
        else if (strcmp(choix, "clear") == 0)
        {
            system("clear");
        }
        else if (strcmp(choix, "crypt") == 0)
        {
            char in[TAILLE_MAX_NOM_FICHIER];
            char out[TAILLE_MAX_NOM_FICHIER];
            int idCle;
            sscanf(commande, "%s %s %s %d", choix, in, out, &idCle);
            keyIdentifier *keyStruct = getKeyWithID(mainKeyList, idCle);
            if (keyStruct != NULL)
            {
                rsaKey_t public = keyStruct->public;
                fichier_chiffrement(in, out, &public);
            }
        }
        else if (strcmp(choix, "uncrypt") == 0)
        {
            char in[TAILLE_MAX_NOM_FICHIER];
            char out[TAILLE_MAX_NOM_FICHIER];
            int idCle;
            sscanf(commande, "%s %s %s %d", choix, in, out, &idCle);
            keyIdentifier *keyStruct = getKeyWithID(mainKeyList, idCle);
            if (keyStruct != NULL)
            {
                rsaKey_t private = keyStruct->private;
                fichier_dechiffrement(in, out, &private);
            }
        }
        else
        {
            printf("Entree invalide\n");
        }
    } while (quitter != 'y');

    list_delete(&mainKeyList);
    printf("Exit success\n");

    return 0;
}