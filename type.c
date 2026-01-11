#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "type.h"

void initPetTypeList(petTypeList *l){
    l->head = NULL;
    l->tail = NULL;
    l->counter = 0;
}

void loadDataPetType(petTypeList *l){
    FILE *file = fopen("tipo_pet.bin", "rb");
    petType new;
    if(file){
        while(fread(&new, sizeof(petType), 1, file)){
            insertPetType(l, new);
        }
        fclose(file);
    } else {
        printf("Erro: Arquivo 'tipo_pet' não encontrado!\n");
    }
}

void insertPetType(petTypeList *l, petType element){
    petTypeNode *new = malloc(sizeof(petTypeNode));
    new->element = element;
    new->next = NULL;
    if(!l->head){
        new->previous = NULL;
        l->head = new;
        l->tail = new;
    } else {
        new->previous = l->tail;
        l->tail->next = new;
        l->tail = new;
    }
    l->counter++;
}

int registerPetType(petTypeList *l, int code, char description[50]){
    petType new;
    if(!searchPetType(l, code)){
        new.code = code;
    } else {
        printf("Erro: Já existe TIPO DE PET com código %d!\n", code);
        return 0;
    }
    strncpy(new.description, description, sizeof(new.description) - 1);
    new.description[sizeof(new.description) - 1] = '\0';

    insertPetType(l, new);
    return 1;
}

petTypeNode* searchPetType(petTypeList *l, int code){
    petTypeNode *node = l->head;
    while(node && node->element.code != code){
        node = node->next;
    }
    return node;
}

void showPetType(petTypeNode *node){
    printf("\n--- TIPO DE PET ---\n");
    printf("Código: %d\n", node->element.code);
    printf("Descrição: %s\n", node->element.description);
}

int deletePetType(petTypeList *l, int code){
    petTypeNode *node = searchPetType(l, code);
    if(!node){
        printf("Erro: TIPO DE PET com código %d não encontrado!\n", code);
        return 0;
    } else if(node == l->head && node == l->tail){
        l->head = NULL;
        l->tail = NULL;
    } else if(node == l->head) {
        node->next->previous = NULL;
        l->head = node->next;
    } else if(node == l->tail){
        node->previous->next = NULL;
        l->tail = node->previous;
    } else {
        node->previous->next = node->next;
        node->next->previous = node->previous;
    }
    free(node);
    return 1;
}

int updatePetType(petTypeList *l, int code, char description[50]){
    petTypeNode *node = searchPetType(l, code);
    if(!node){
        printf("Erro: TIPO DE PET com código %d não encontrado!\n", code);
        return 0;
    }
    if(strcmp(description, "") != 0){
        strncpy(node->element.description, description, sizeof(node->element.description) - 1);
        node->element.description[sizeof(node->element.description) - 1] = '\0';
    }
    return 1;
}

void freePetTypeList(petTypeList *l){
    petTypeNode *node = l->head;
    while(node){
        petTypeNode *current = node;
        node = node->next;
        free(current);
    }
    l->head = NULL;
    l->tail = NULL;
    l->counter = 0;
}

petTypeTreeNode* insertPetTypeInTree(petTypeTreeNode *root, petType element, int rule){ //Rules de acordo com o critério de ordenamento
    if (!root) {
        petTypeTreeNode *new = malloc(sizeof(petTypeTreeNode));
        new->element = element;
        new->left = NULL;
        new->right = NULL;
        return new;
    }

    int comparation = 0;
    switch (rule){
    case 1: //Comparação por código
        comparation = element.code - (root)->element.code;
        break;
    case 2: //Comparação por descrição
        comparation = strcmp(element.description, (root)->element.description);
        break;
    default:
        return root;
    }

    if(comparation < 0){
        root->left = insertPetTypeInTree(root->left, element, rule);
    } else {
        root->right = insertPetTypeInTree(root->right, element, rule);
    }

    return root;
}

void showPetTypeTreeInOrder(petTypeTreeNode *root){
    if (root != NULL) {
        showPetTypeTreeInOrder(root->left);
        printf("\n--- TIPO DE PET ---\n");
        printf("Código: %d\n", root->element.code);
        printf("Descrição: %s\n", root->element.description);
        showPetTypeTreeInOrder(root->right);
    }
}

void freePetTypeTree(petTypeTreeNode *root){
    if (root != NULL) {
        freePetTypeTree(root->left);
        freePetTypeTree(root->right);
        free(root);
    }
}

void saveDataPetType(petTypeList *l){
    FILE *file = fopen("tipo_pet.bin", "wb");
    if(!file){
        printf("Erro ao abrir arquivo 'tipo_pet.bin' para salvar!\n");
        return;
    }

    petTypeNode *node = l->head;
    while(node != NULL){
        fwrite(&node->element, sizeof(petType), 1, file);
        node = node->next;
    }

    fclose(file);

    printf("Dados de TIPO DE PET salvos com sucesso!\n");
}