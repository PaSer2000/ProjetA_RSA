/*------------------------------------
    Phase 3 : Interprete de commande
------------------------------------*/

#include "rsa_common_header.h"
#include <string.h>
#include <unistd.h>
#define TAILLE_MAX_NOM_FICHIER 30
#define TAILLE_MAX_COMMANDE 100

//------------------------------------Structure dynamique pour stocker les clés---------------------------------------
typedef struct{
    rsaKey_t public;
    rsaKey_t private;
    int id;
}keyIdentifier;

typedef struct s_node{
    struct s_node* next;
    struct s_node* previous;
    keyIdentifier* keyStruct;
}node;

typedef struct{
    node *sentinel;
    int size;
}keysDynamicList;

// Fonction pour créer la liste dynamique de clés
keysDynamicList* list_create(void) {
    keysDynamicList* l = malloc(sizeof(keysDynamicList));
    l->size = 0;
    l->sentinel = malloc(sizeof(node));
    l->sentinel->next = l->sentinel;
    l->sentinel->previous = l->sentinel;
    return l;
}

// Fonction pour supprimer la liste dynamique de clés
void list_delete(keysDynamicList** l) {
    node* element;
    while((*l)->sentinel->next != (*l)->sentinel){
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
keyIdentifier* createNewKeys(rsaKey_t public, rsaKey_t private, int id) {
    keyIdentifier *new = malloc(sizeof(keyIdentifier));
    new->id = id;
    new->private = private;
    new->public = public;
    return new;
}

// Ajouter une clé à la liste
keysDynamicList* list_push_back(keysDynamicList* l, keyIdentifier* keys) {
    node* newElement = malloc(sizeof(node));
    newElement->keyStruct = keys;
    l->sentinel->previous->next = newElement;
    newElement->next = l->sentinel;
    newElement->previous = l->sentinel->previous;
    l->sentinel->previous = newElement;
    l->size += 1;
    return l;
}

// Lister toutes les clés
keysDynamicList* list_map(keysDynamicList* l) {
    node* element = l->sentinel->next;
    while (element != l->sentinel) {
        printf("Paire n° %d : ", element->keyStruct->id);
        affichageClefs(&(element->keyStruct->public), &(element->keyStruct->private));
        element = element->next;
    }
    return l;
}

// Supprimer une clé par son identifiant
keysDynamicList* supprimerCle(keysDynamicList* l, int id) {
    node* element = l->sentinel->next;
    while (element != l->sentinel) {
        if (element->keyStruct->id == id) {
            element->previous->next = element->next;
            element->next->previous = element->previous;
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

// Fonction de sauvegarde des clés dans un fichier sécurisé
void sauvegarderCle(keysDynamicList* keyList, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }
    fclose(file);
}

// Fonction de nettoyage du tampon d'entrée
void clearBuffer() {
    int clean;
    while ((clean = getchar() != '\n') && clean != EOF);
}
void genererPairCle(keysDynamicList* keyList,int id)
{
    printf("Generation des clés publique et privee...\n");
    rsaKey_t publicKey, privateKey;
    genKeysRabin(&publicKey, &privateKey, MAX_PRIME);
    printf("La paire de cle a ete generee :\n");
    affichageClefs(&publicKey, &privateKey);
    list_push_back(keyList, createNewKeys(publicKey, privateKey,id));
    list_map(keyList);
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
int main() {
    keysDynamicList* mainKeyList = list_create();
    char commande[TAILLE_MAX_COMMANDE];
    char choix[50];
    char quitter;
    do {
        printf("===== Bienvenue sur l'interprete de commande =====\n");
        printf("Que veux-tu faire ?\n");
        printf("1-Quitter le terminal\n");
        printf("2-Ouvrir l'aide ?\n");
        printf("3-Générer une nouvelle clé ?\n");
        printf("4-Convertir un fichier binaire en b64 ?\n");
        printf("5-Convertir un fichier b64 en binaire ?\n");
        printf("6-Supprimez une clé\n");
        printf("7-Sauvegardez les clés\n");

        fgets(commande, TAILLE_MAX_COMMANDE, stdin);
        sscanf(commande, "%s", choix);

        if (strcmp(choix, "1") == 0) {
            printf("Voulez-vous quitter le terminal ? y/n : ");
            do {
                fscanf(stdin, "%c", &quitter);
                clearBuffer();
                if (quitter != 'y' && quitter != 'n') {
                    printf("Entrez 'y' ou 'n'\n");
                }
            } while (quitter != 'y' && quitter != 'n');
        }
        else if (strcmp(choix, "2") == 0) {
            printf("https://moodle.univ-tlse3.fr/pluginfile.php/1126789/mod_folder/content/0/interpre%%CC%%80te.pdf?forcedownload=1\n");
        }
        else if (strcmp(choix, "3") == 0) {
            int id;
            printf("Entrez l'identifiant de la clé : ");
            fgets(commande, TAILLE_MAX_COMMANDE, stdin);
            sscanf(commande, "%d", &id);
            genererPairCle(mainKeyList, id);
        }
        else if (strcmp(choix, "4") == 0) {
            char in[TAILLE_MAX_NOM_FICHIER], out[TAILLE_MAX_NOM_FICHIER];
            printf("Entrez le fichier d'entrée (binaire) : ");
            fgets(in, TAILLE_MAX_NOM_FICHIER, stdin);
            printf("Entrez le fichier de sortie (base64) : ");
            fgets(out, TAILLE_MAX_NOM_FICHIER, stdin);
            convertb64ToBinary(in, out);
        }
        else if (strcmp(choix, "5") == 0) {
            char in[TAILLE_MAX_NOM_FICHIER], out[TAILLE_MAX_NOM_FICHIER];
            printf("Entrez le fichier d'entrée (base64) : ");
            fgets(in, TAILLE_MAX_NOM_FICHIER, stdin);
            printf("Entrez le fichier de sortie (binaire) : ");
            fgets(out, TAILLE_MAX_NOM_FICHIER, stdin);
            convertb64ToBinary(in, out);
        }
        else if (strcmp(choix, "6") == 0) {
            int id;
            printf("Entrez l'identifiant de la clé à supprimer : ");
            fgets(commande, TAILLE_MAX_COMMANDE, stdin);
            sscanf(commande, "%d", &id);
            mainKeyList = supprimerCle(mainKeyList, id);
        }
        else if (strcmp(choix,"7") == 0) {
            sauvegarderCle(mainKeyList, "clé.txt");
        }
        else {
            printf("Entrée invalide\n");
        }
        sleep(1);
        printf("\n\n\n\n");
    } while (quitter != 'y');
    
    list_delete(&mainKeyList);
    printf("Exit success\n");

    return 0;
}