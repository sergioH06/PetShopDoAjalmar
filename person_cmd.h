#ifndef PERSON_CMD_H
#define PERSON_CMD_H

#include "person.h"
#include "pet.h"
#include "util.h"

void validateInsertPerson(personList *listPerson, char *cursor);

void validateUpdatePerson(personList *listPerson, char *cursor);

void validateDeletePerson(personList *listPerson, petList *listPet, char *cursor);

void validateSelectPerson(personList *listPerson, char *cursor);

#endif