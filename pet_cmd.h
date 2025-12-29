#ifndef PET_CMD_H
#define PET_CMD_H

#include "person.h"
#include "type.h"
#include "pet.h"

void validateInsertPet(petList *listPet, personList *listPerson, petTypeList *listType, char *cursor);

void validateUpdatePet(petList *listPet, personList *listPerson, petTypeList *listType, char *cursor);

void validateDeletePet(petList *listPet, char *cursor);

void validateSelectPet(petList *listPet, char *cursor);

#endif