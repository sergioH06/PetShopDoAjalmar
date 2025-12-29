#ifndef PET_CMD_H
#define PET_CMD_H

#include "person.h"
#include "type.h"
#include "pet.h"

void executeInsertPet(petList *listPet, personList *listPerson, petTypeList *listType, char *cursor);

void executeUpdatePet(petList *listPet, personList *listPerson, petTypeList *listType, char *cursor);

void executeDeletePet(petList *listPet, char *cursor);

void executeSelectPet(petList *listPet, char *cursor);

#endif