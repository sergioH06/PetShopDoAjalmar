#ifndef TYPE_CMD_H
#define TYPE_CMD_H

#include "type.h"
#include "pet.h"
#include "util.h"

void validateInsertPetType(petTypeList *listType, char *cursor);

void validateUpdatePetType(petTypeList *listType, char *cursor);

void validateDeletePetType(petTypeList *listType, petList *listPet, char *cursor);

void validateSelectPetType(petTypeList *listType, char *cursor);

#endif