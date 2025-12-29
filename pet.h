#ifndef PET_H
#define PET_H

#include "person.h"
#include "type.h"

typedef struct {
    int code;
    char name[50];
    int personCode;
    int petTypeCode;
} pet;

typedef struct petNode {
    pet element;
    struct petNode *next;
    struct petNode *previous;
} petNode;

typedef struct {
    petNode *head;
    petNode *tail;
    int counter;
} petList;

typedef struct petTreeNode {
    pet element;
    struct petTreeNode *left;
    struct petTreeNode *right;
} petTreeNode;

void initPetList(petList *l);

void loadDataPet(petList *l);

void insertPet(petList *l, pet element);

void registerPet(petList *listPet, personList *listPerson, petTypeList *listType, int code, char name[50], int personCode, int petTypeCode);

petNode* searchPet(petList *l, int code);

void showPet(petNode *node);

void deletePet(petList *l, int code);

void updatePet(petList *listPet, personList *listPerson, petTypeList *listType, int code, char name[50], int personCode, int petTypeCode);

void freePetList(petList *l);

void deletePetByPerson(petList *l, int personCode);

void deletePetByType(petList *l, int petTypeCode);

void showAll(personList *listPerson, petList *listPet, petTypeList *listType, int personCode);

petTreeNode* insertPetInTree(petTreeNode *root, pet element, int rule);

void showPetTreeInOrder(petTreeNode *root);

void freePetTree(petTreeNode *root);

void saveDataPet(petList *l);

#endif