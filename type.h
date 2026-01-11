#ifndef TYPE_H
#define TYPE_H

typedef struct {
    int code;
    char description[50];
} petType;

typedef struct petTypeNode {
    petType element;
    struct petTypeNode *next;
    struct petTypeNode *previous;
} petTypeNode;

typedef struct {
    petTypeNode *head;
    petTypeNode *tail;
    int counter;
} petTypeList;

typedef struct petTypeTreeNode {
    petType element;
    struct petTypeTreeNode *left;
    struct petTypeTreeNode *right;
} petTypeTreeNode;

void initPetTypeList(petTypeList *l);

void loadDataPetType(petTypeList *l);

void insertPetType(petTypeList *l, petType element);

int registerPetType(petTypeList *l, int code, char description[50]);

petTypeNode* searchPetType(petTypeList *l, int code);

void showPetType(petTypeNode *node);

int deletePetType(petTypeList *l, int code);

void freePetTypeList(petTypeList *l); 

int updatePetType(petTypeList *l, int code, char description[50]);

petTypeTreeNode* insertPetTypeInTree(petTypeTreeNode *root, petType element, int rule);

void showPetTypeTreeInOrder(petTypeTreeNode *root);

void freePetTypeTree(petTypeTreeNode *root);

void saveDataPetType(petTypeList *l);

#endif