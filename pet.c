#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pet.h"

void initPetList(petList *l){
    l->head = NULL;
    l->tail = NULL;
    l->counter = 0;
}

void loadDataPet(petList *l){
    FILE *file = fopen("pet.bin", "rb");
    pet new;
    if(file){
        while(fread(&new, sizeof(pet), 1, file)){
            insertPet(l, new);
        }
        fclose(file);
    } else {
        printf("Erro: Arquivo 'pet.bin' não encontrado!\n");
    }
}

void insertPet(petList *l, pet element){
    petNode *new = malloc(sizeof(petNode));
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

int registerPet(petList *listPet, personList *listPerson, petTypeList *listType, int code, char name[50], int personCode, int petTypeCode){
    pet new;
    if(!searchPerson(listPerson, personCode)){
        printf("Erro: Não existe PESSOA com código %d!\n", personCode);
        return 0;
    }
    if(!searchPetType(listType, petTypeCode)){
        printf("Erro: Não existe TIPO DE PET com código!\n", petTypeCode);
        return 0;
    }
    if(searchPet(listPet, code)){
        printf("Erro: Já existe PET com código %d!\n", code);
        return 0;
    }
    
    strncpy(new.name, name, sizeof(new.name) - 1);
    new.name[sizeof(new.name) - 1] = '\0';
    new.personCode = personCode;   
    new.petTypeCode = petTypeCode; 
    new.code = code;

    insertPet(listPet, new);
    return 1;
}

petNode* searchPet(petList *l, int code){
    petNode *node = l->head;
    while(node && node->element.code != code){
        node = node->next;
    }
    return node;
}

void showPet(petNode *node){
    printf("\n--- PET ---\n");
    printf("Código: %d\n", node->element.code);
    printf("Nome: %s\n", node->element.name);
    printf("Código do Dono: %d\n", node->element.personCode);
    printf("Código de Tipo: %d\n", node->element.petTypeCode);
}

int deletePet(petList *l, int code){
    petNode *node = searchPet(l, code);
    if(!node){
        printf("Erro: PET com código %d não encontrado!\n", code);
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

int updatePet(petList *listPet, personList *listPerson, petTypeList *listType, int code, char name[50], int personCode, int petTypeCode){
    petNode *node = searchPet(listPet, code);
    if(!node){
        printf("Erro: PET com código %d não encontrado!\n", code);
        return 0;
    }
    if(strcmp(name, "") != 0){
        strncpy(node->element.name, name, sizeof(node->element.name) - 1);
        node->element.name[sizeof(node->element.name) - 1] = '\0';
    }
    if(personCode != -1){
        if(searchPerson(listPerson, personCode)){
            node->element.personCode = personCode;
        } else {
            printf("Código %d para PESSOA inválido!\n", personCode);
            return 0;
        }
    }
    if(petTypeCode != -1){
        if(searchPetType(listType, petTypeCode)){
            node->element.petTypeCode = petTypeCode;
        } else {
            printf("Erro: Código %d para TIPO DE PET inválido!\n", petTypeCode);
            return 0;
        }
    }
    return 1;
}

void freePetList(petList *l){
    petNode *node = l->head;
    while(node){
        petNode *current = node;
        node = node->next;
        free(current);
    }
    l->head = NULL;
    l->tail = NULL;
    l->counter = 0;
}

void showAll(personList *listPerson, petList *listPet, petTypeList *listType, int personCode){
    personNode *nodePerson = searchPerson(listPerson, personCode);
    showPerson(nodePerson);
    petNode *node = listPet->head;
    while(node){
        if(node->element.personCode == personCode){
            showPet(node);
            petTypeNode *nodePetType = searchPetType(listType, node->element.petTypeCode);
            showPetType(nodePetType);
        }
        node = node->next;
    }
}

void deletePetByPerson(petList *l, int personCode){
    petNode *current = l->head;
    petNode *node;
    while(current){
        node = current->next;
        if(current->element.personCode == personCode){
            deletePet(l, current->element.code);
        }
        current = node;
    }
}

void deletePetByType(petList *l, int petTypeCode){
    petNode *current = l->head;
    petNode *node;
    while(current){
        node = current->next;
        if(current->element.petTypeCode == petTypeCode){
            deletePet(l, current->element.code);
        }
        current = node;
    }
}

petTreeNode* insertPetInTree(petTreeNode *root, pet element, int rule){ //rules de acordo com o critério de ordenamento
    if (!root) {
        petTreeNode *new = malloc(sizeof(petTreeNode));
        new->element = element;
        new->left = NULL;
        new->right = NULL;
        return new;
    }

    int comparation = 0;
    switch (rule){
    case 1: //comparação por código
        comparation = element.code - (root)->element.code;
        break;
    case 2: //comparação por nome
        comparation = strcmp(element.name, (root)->element.name);
        break;
    case 3: //comparação por código de dono?
        comparation = element.personCode - (root)->element.personCode;
        break;
    case 4: //comparação por tipo de pet? pra que
        comparation = element.petTypeCode - (root)->element.petTypeCode;
        break;
    default:
        return root;
    }

    if(comparation < 0){
        root->left = insertPetInTree(root->left, element, rule);
    } else {
        root->right = insertPetInTree(root->right, element, rule);
    }

    return root;
}

void showPetTreeInOrder(petTreeNode *root){
    if (root != NULL) {
        showPetTreeInOrder(root->left);
        printf("\n--- PET ---\n");
        printf("Código: %d\n", root->element.code);
        printf("Nome: %s\n", root->element.name);
        printf("Código do Dono: %d\n", root->element.personCode);
        printf("Código de Tipo: %d\n", root->element.petTypeCode);
        showPetTreeInOrder(root->right);
    }
}

void freePetTree(petTreeNode *root){
    if (root != NULL) {
        freePetTree(root->left);
        freePetTree(root->right);
        free(root);
    }
}

void saveDataPet(petList *l){
    FILE *file = fopen("pet.bin", "wb");
    if(!file){
        printf("Erro ao abrir arquivo 'pet.bin' para salvar!\n");
        return;
    }

    petNode *node = l->head;
    while(node != NULL){
        fwrite(&node->element, sizeof(pet), 1, file);
        node = node->next;
    }

    fclose(file);

    printf("Dados de PET salvos com sucesso!\n");
}