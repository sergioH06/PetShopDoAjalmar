#ifndef PERSON_CMD_H
#define PERSON_CMD_H

#include "person.h"
#include "pet.h"
#include "util.h"

void executeInsertPerson(personList *listPerson, char *cursor);

void executeUpdatePerson(personList *listPerson, char *cursor);

void executeDeletePerson(personList *listPerson, petList *listPet, char *cursor);

void executeSelectPerson(personList *listPerson, char *cursor);

#endif