#ifndef PERSON_H
#define PERSON_H

typedef struct {
    int code;
    char name[100];
    char address[50];
    char phone[20];
    char birthDate[11];
} person;

typedef struct personNode {
    person element;
    struct personNode *next;
    struct personNode *previous;
} personNode;

typedef struct {
    personNode *head;
    personNode *tail;
    int counter;
} personList;

typedef struct personTreeNode {
    person element;
    struct personTreeNode *left;
    struct personTreeNode *right;
} personTreeNode;

void initPersonList(personList *l);

void loadDataPerson(personList *l);

void insertPerson(personList *l, person element);

int registerPerson(personList *l, int code, char name[100], char address[50], char phone[20], char birthDate[11]);

personNode* searchPerson(personList *l, int code);

void showPerson(personNode *node);

int deletePerson(personList *l, int code);

int updatePerson(personList *l, int code, char name[100], char address[50], char phone[20], char birthDate[11]);

void freePersonList(personList *l);

personTreeNode* insertPersonInTree(personTreeNode *root, person element, int rule);

void showPersonTreeInOrder(personTreeNode *root);

void freePersonTree(personTreeNode *root);

void saveDataPerson(personList *l);

#endif