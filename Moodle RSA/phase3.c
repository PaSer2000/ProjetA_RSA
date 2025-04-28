/*------------------------------------
    Phase 3 : Interprete de commande
------------------------------------*/

#include "rsa_common_header.h"
#include <string.h>
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



keysDynamicList* list_create(void) {
	keysDynamicList* l = malloc(sizeof(keysDynamicList));
	l->size = 0;
	l->sentinel = malloc(sizeof(node));
	l->sentinel->next = l->sentinel;
	l->sentinel->previous = l->sentinel;
	return l;
}

void list_delete(keysDynamicList** l) {
	node* element;
	while((*l)->sentinel->next != (*l)->sentinel){
		element = (*l)->sentinel->next;
		(*l)->sentinel->next = element->next;
        free(element->keyStruct);
		free(element);
		(*l)->size -= 1;
	}
	free(((*l)->sentinel)); //ou element = (*l)-> sentinel et free(element)
	free(*l);
	*l=NULL;
}

keyIdentifier* createNewKeys(rsaKey_t public, rsaKey_t private, int id){
    keyIdentifier *new = malloc(sizeof(keyIdentifier));
    new->id = id;
    new ->private = private;
    new->public = public;
    return new;
}

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

keysDynamicList* list_map(keysDynamicList* l) {
	node* element = l->sentinel->next;
	while (element != l->sentinel)
	{
		printf("Paire n° %d : ", element->keyStruct->id);
        affichageClefs(&(element->keyStruct->public), &(element->keyStruct->private));
        element = element->next;
	}
	return l;
}

//--------------------------------------------------------------------------------------------------------------------

void clearBuffer()
{
    // Vidage de la memoire tampon pour clear stdin
    int clean;
    while ((clean = getchar() != '\n') && clean != EOF)
        ;
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

int main()
{
    keysDynamicList* mainKeyList = list_create();
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
            char idChar[10];
            int id;
            sscanf(commande, "%s %s", choix, idChar);
            id = atoi(idChar);
            genererPairCle(mainKeyList, id);
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
    list_delete(&mainKeyList);
    printf("Exit success\n");
}