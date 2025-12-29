#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "person.h"
#include "util.h"

void initPersonList(personList *l){
    l->head = NULL;
    l->tail = NULL;
    l->counter = 0;
}

void loadDataPerson(personList *l){
    FILE *file = fopen("pessoa.bin", "rb");
    person new;
    if(file){
        while(fread(&new, sizeof(person), 1, file)){
            insertPerson(l, new);
        }
        fclose(file);
    } else {
        printf("Erro: Arquivo 'pessoa.bin' não encontrado!\n");
    }
}

void insertPerson(personList *l, person element){
    personNode *new = malloc(sizeof(personNode));
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

int registerPerson(personList *l, int code, char name[100], char address[50], char phone[20], char birthDate[11]){
    person new;
    if(!searchPerson(l, code)){
        new.code = code;
    } else {
        printf("Erro: Já existe PESSOA com código %d!\n", code);
        return 0;
    }

    strncpy(new.name, name, sizeof(new.name) - 1);
    new.name[sizeof(new.name) - 1] = '\0';
    strncpy(new.address, address, sizeof(new.address) - 1);
    new.address[sizeof(new.address) - 1] = '\0';
    strncpy(new.phone, phone, sizeof(new.phone) - 1);
    new.phone[sizeof(new.phone) - 1] = '\0';
    if(validateBirthDate(birthDate)){
        strncpy(new.birthDate, birthDate, sizeof(new.birthDate) - 1);
        new.birthDate[sizeof(new.birthDate) - 1] = '\0';
    } else {
        return 0;
    }

    insertPerson(l, new);
    return 1;
}

personNode* searchPerson(personList *l, int code){
    personNode *node = l->head;
    while(node && node->element.code != code){
        node = node->next;
    }
    return node;
}

void showPerson(personNode *node){
    printf("\n--- PESSOA ---\n");
    printf("Código: %d\n", node->element.code);
    printf("Nome: %s\n", node->element.name);
    printf("Endereço: %s\n", node->element.address);
    printf("Telefone: %s\n", node->element.phone);
    printf("Data de Nascimento: %s\n", node->element.birthDate);
}

int deletePerson(personList *l, int code){
    personNode *node = searchPerson(l, code);
    if(!node){
        printf("Erro: PESSOA com código %d não encontrado!\n", code);
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

int updatePerson(personList *l, int code, char name[100], char address[50], char phone[20], char birthDate[11]){
    personNode *node = searchPerson(l, code);
    if(!node){
        printf("Erro: PESSOA com código %d não encontrado!\n", code);
        return 0;
    }
    if(strcmp(name, "") != 0){
        strncpy(node->element.name, name, sizeof(node->element.name) - 1);
        node->element.name[sizeof(node->element.name) - 1] = '\0';
    }
    if(strcmp(address, "") != 0){
        strncpy(node->element.address, address, sizeof(node->element.address) - 1);
        node->element.address[sizeof(node->element.address) - 1] = '\0';
    }
    if(strcmp(phone, "") != 0){
        strncpy(node->element.phone, phone, sizeof(node->element.phone) - 1);
        node->element.phone[sizeof(node->element.phone) - 1] = '\0';
    }
    if(strcmp(birthDate, "") != 0 && validateBirthDate(birthDate)){
        strncpy(node->element.birthDate, birthDate, sizeof(node->element.birthDate) - 1);
        node->element.birthDate[sizeof(node->element.birthDate) - 1] = '\0';
    } else {
        return 0;
    }
    return 1;
}

void freePersonList(personList *l){
    personNode *node = l->head;
    while(node){
        personNode *current = node;
        node = node->next;
        free(current);
    }
    l->head = NULL;
    l->tail = NULL;
    l->counter = 0;
}

personTreeNode* insertPersonInTree(personTreeNode *root, person element, int rule){ //rules de acordo com o critério de ordenamento
    if (!root) {
        personTreeNode *new = malloc(sizeof(personTreeNode));
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
    case 3: //comparação por endereço
        comparation = strcmp(element.address, (root)->element.address);
        break;
    case 4: //comparação por telefone
        comparation = strcmp(element.phone, (root)->element.phone);
        break;
    case 5: //comparação por data de nascimento
        comparation = compareDates(element.birthDate, (root)->element.birthDate);
        break;
    default:
        return root;
    }

    if(comparation < 0){
        root->left = insertPersonInTree(root->left, element, rule);
    } else {
        root->right = insertPersonInTree(root->right, element, rule);
    }

    return root;
}

void showPersonTreeInOrder(personTreeNode *root){
    if (root != NULL) {
        showPersonTreeInOrder(root->left);
        printf("\n--- PESSOA ---\n");
        printf("Código: %d\n", root->element.code);
        printf("Nome: %s\n", root->element.name);
        printf("Endereço: %s\n", root->element.address);
        printf("Telefone: %s\n", root->element.phone);
        printf("Data de Nascimento: %s\n", root->element.birthDate);
        showPersonTreeInOrder(root->right);
    }
}

void freePersonTree(personTreeNode *root){
    if (root != NULL) {
        freePersonTree(root->left);
        freePersonTree(root->right);
        free(root);
    }
}

void saveDataPerson(personList *l){
    FILE *file = fopen("pessoa.bin", "wb");
    if (!file){
        printf("Erro ao abrir arquivo 'pessoa.bin' para salvar!\n");
        return;
    }

    personNode *node = l->head;
    while (node != NULL){
        fwrite(&node->element, sizeof(person), 1, file);
        node = node->next;
    }

    fclose(file);
    
    printf("Dados de PESSOA salvos com sucesso!\n");
}