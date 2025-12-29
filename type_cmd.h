#ifndef TYPE_CMD_H
#define TYPE_CMD_H

#include "type.h"
#include "pet.h"
#include "util.h"

void executeInsertPetType(petTypeList *listType, char *cursor);

void executeUpdatePetType(petTypeList *listType, char *cursor);

void executeDeletePetType(petTypeList *listType, petList *listPet, char *cursor);

void executeSelectPetType(petTypeList *listType, char *cursor);

#endif