#ifndef COMMAND_H
#define COMMAND_H

#include "person.h"
#include "type.h"
#include "pet.h"

typedef struct commandNode {
    char command[500];
    struct commandNode *next;
    struct commandNode *previous;
} commandNode;

typedef struct {
    commandNode *head;
    commandNode *tail;
    int size;
} commandQueue;

void initCommandQueue(commandQueue *q);

void enqueueCommand(commandQueue *q, char command[500]);

commandNode* dequeueCommand(commandQueue *q);

void readScript(commandQueue *q, const char *filename);

void freeCommandQueue(commandQueue *q);

void processCommands(commandQueue *queueCommand, personList *listPerson, petTypeList *listType, petList *listPet);

#endif