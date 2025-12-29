#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pet.h"
#include "type.h"
#include "person.h"
#include "util.h"

void validateInsertPet(petList *listPet, personList *listPerson, petTypeList *listType, char *cursor){
    printf("\n--- INSERT em PET ---\n");
    char *next = NULL;
    int map[10];
    int counter = 0;

    int code = -1;
    int personCode = -1;
    char name[50] = "";
    int petTypeCode = -1;

    cursor = skipWhitespace(cursor);

    if(*cursor == '('){
        cursor++;

        while(*cursor && *cursor != ')'){
            char field[50];
            if(!readToken(&cursor, field)) return;

            if(counter >= 4){
                printf("Erro: Excesso de campos!\n");
                return;
            }
            
            if(equals(field, "codigo") == 0) map[counter] = 0;
            else if(equals(field, "codigo_pes") == 0) map[counter] = 1;
            else if(equals(field, "nome") == 0) map[counter] = 2;
            else if(equals(field, "codigo_tipo") == 0) map[counter] = 3;
            else {
                printf("Erro: Campo desconhecido '%s' para INSERT em PET!\n", field);
                return;
            }
            counter++;

            cursor = skipWhitespace(cursor);

            if(*cursor == ','){
                cursor++;
            } else if(*cursor != ')'){
                printf("Erro: Sintaxe inválida. Esperada vírgula ',' separando as colunas.\n");
                return;
            }
        }
        if(*cursor == ')') cursor++;
    }

    cursor = skipWhitespace(cursor);

    if((next = checkKeyword(cursor, "values")) != NULL){
        cursor = next;
        cursor = skipWhitespace(cursor);

        if(*cursor == '('){
            cursor++;

            for(int i = 0; i < counter; i++){
                char value[100];
                int type = readToken(&cursor, value);
                if(type == 0) return;

                switch(map[i]){
                    case 0:
                        if(type == 2){
                            printf("Erro: 'codigo' deve ser um número!\n");
                            return;
                        }
                        code = atoi(value);
                        break;
                    case 1:
                        if(type == 2){
                            printf("Erro: 'codigo_pes' deve ser um número!\n");
                            return;
                        }
                        personCode = atoi(value);
                        break;
                    case 2:
                        if(type != 2){
                            printf("Erro: 'nome' deve ser uma string!\n");
                            return;
                        }
                        strncpy(name, value, 49);
                        name[49] = '\0';
                        break;
                    case 3:
                        if(type == 2){
                            printf("Erro: 'codigo_tipo' deve ser um número!\n");
                            return;
                        }
                        petTypeCode = atoi(value);
                        break;
                }
                cursor = skipWhitespace(cursor);

                if(*cursor == ','){
                    cursor++;
                } else if(*cursor != ')'){
                    printf("Erro: Sintaxe inválida. Esperada vírgula ',' separando as colunas.\n");
                    return;
                }
            }
            if (*cursor == ')') cursor++;
        }
    }
    int error = 0;
    if (code == -1) {
        printf("Erro: O campo 'codigo' é obrigatório para PET.\n");
        error = 1;
    }
    if(personCode == -1){
        printf("Erro: O campo 'codigo_pes' é obrigatório para PET.\n");
        error = 1;
    }
    if (strcmp(name, "") == 0) {
        printf("Erro: O campo 'nome' é obrigatório para PET.\n");
        error = 1;
    }
    if(petTypeCode == -1){
        printf("Erro: O campo 'codigo_tipo' é obrigatório para PET.\n");
        error = 1;
    }

    if(!error){
        if(registerPet(listPet, listPerson, listType, code, name, personCode, petTypeCode)){
            printf("Comando INSERT processado para PET com código %d.\n", code);
        }
    }
}

void validateUpdatePet(petList *listPet, personList *listPerson, petTypeList *listType, char *cursor){
    printf("\n--- UPDATE em PET ---\n");
    char *next = NULL;
    
    int targetCode = -1;
    char name[50] = "";
    int personCode = -1;
    int petTypeCode = -1;

    cursor = skipWhitespace(cursor);

    if((next = checkKeyword(cursor, "set ")) == NULL){
        printf("Erro: Sintaxe inválida. Cláusula SET inexistente!\n");
        return;
    }

    cursor = next;
    cursor = skipWhitespace(cursor);

    while(*cursor && checkKeyword(cursor, "where") == NULL){
        char field[50];
        char value[70];

        if(!readToken(&cursor, field)) return;

        cursor = skipWhitespace(cursor);

        if(*cursor != '='){
            printf("Erro: Sintaxe inválida. Esperado '='!\n");
            return;
        }
        cursor++;

        int type = readToken(&cursor, value);
        if(type == 0) return;

        if(equals(field, "nome") == 0){
            if(type != 2){
                printf("Erro: 'nome' deve ser uma string!\n");
                return;
            }
            strncpy(name, value, 49); // Faltava isso
            name[49] = '\0';
        } else if(equals(field, "codigo_pes") == 0){
            if(type == 2){
                printf("Erro: 'codigo_pes' deve ser um número!\n");
                return;
            }
            personCode = atoi(value);
        } else if(equals(field, "codigo_tipo") == 0){
            if(type == 2){
                printf("Erro: 'codigo_tipo' deve ser um número!\n");
                return;
            }
            petTypeCode = atoi(value);
        }else {
            printf("Erro: Campo %s desconhecido ou inválido para UPDATE em PET.\n", field);
            return;
        }

        cursor = skipWhitespace(cursor);

        if (*cursor == '\0' || checkKeyword(cursor, "where") != NULL) {
            break; 
        } 
        if (*cursor == ',') {
            cursor++;
        } else {
            printf("Erro: Sintaxe inválida. Esperada vírgula ',' separando os campos do SET.\n");
            return;
        } 
    }
    
    if((next = checkKeyword(cursor, "where ")) == NULL){
        printf("Erro: Sintaxe inválida. Cláusula WHERE inexistente!\n");
        return;
    }
    cursor = next;
    cursor = skipWhitespace(cursor);

    char where[50];
    if(!readToken(&cursor, where)) return;

    if(equals(where, "codigo") != 0 && equals(where, "id") != 0){
        printf("Erro: A cláusula WHERE deve usar o campo 'codigo'!\n");
        return;
    }
    
    cursor = skipWhitespace(cursor);

    if(*cursor != '='){
        printf("Erro: Sintaxe inválida. Esperado '='!\n");
        return;
    }
    cursor++;

    char id[20];
    int typeID = readToken(&cursor, id);
    if(typeID == 2) { 
        printf("Erro: 'codigo' deve ser numérico.\n"); return; 
    }
    targetCode = atoi(id);

    if(updatePet(listPet, listPerson, listType, targetCode, name, personCode, petTypeCode)){
        printf("Comando UPDATE processado para PET com código %d.\n", targetCode);
    }
}

void validateDeletePet(petList *listPet, char *cursor){
    printf("\n--- DELETE em PET ---\n");
    char *next = NULL;
    int targetCode = -1;

    cursor = skipWhitespace(cursor);

    if((next = checkKeyword(cursor, "where ")) == NULL){
        printf("Erro: Sintaxe inválida. Cláusula WHERE inexistente!\n");
        return;
    }

    cursor = next;
    cursor = skipWhitespace(cursor);

    char where[50];
    if(!readToken(&cursor, where)) return;

    if(equals(where, "codigo") != 0){
        printf("Erro: A cláusula WHERE deve usar o campo 'codigo'!\n");
        return;
    }

    cursor = skipWhitespace(cursor);

    if(*cursor != '='){
        printf("Erro: Sintaxe inválida. Esperado '='!\n");
        return;
    }
    cursor++;

    char id[20];
    int typeID = readToken(&cursor, id);
    if(typeID == 2) { 
        printf("Erro: 'codigo' deve ser numérico.\n"); return; 
    }
    targetCode = atoi(id);

    if(deletePet(listPet, targetCode)){
        printf("Comando DELETE processado para PET com código %d.\n", targetCode);
    }
}

void validateSelectPet(petList *listPet, char *cursor){
    printf("\n--- SELECT em PET ---\n");
    char *next = NULL;
    int rule = 0;

    cursor = skipWhitespace(cursor);

    if((next = checkKeyword(cursor, "order ")) != NULL){
        cursor = next;
        cursor = skipWhitespace(cursor);

        if((next = checkKeyword(cursor, "by ")) != NULL){
            cursor = next;
            cursor = skipWhitespace(cursor);

            char field[50];
            if(!readToken(&cursor, field)) return;

            if(equals(field, "codigo") == 0) rule = 1;
            else if(equals(field, "nome") == 0) rule = 2;
            else if(equals(field, "codigo_pes") == 0) rule = 3;
            else if(equals(field, "codigo_tipo") == 0) rule = 4;
            else {
                printf("Erro: Campo %s desconhecido para PET.\n", field);
                return;
            }

            petTreeNode *root = NULL;
            petNode *current = listPet->head;
            while (current != NULL) {
                root = insertPetInTree(root, current->element, rule);
                current = current->next;
            }

            if (root == NULL) {
                printf("Nenhum registro encontrado.\n");
            } else {
                showPetTreeInOrder(root);
            }

            freePetTree(root);
            return;
        }
    } else if((next = checkKeyword(cursor, "where ")) != NULL){
        cursor = next;
        cursor = skipWhitespace(cursor);

        char field[50];
        if(!readToken(&cursor, field)) return;

        if(equals(field, "codigo") != 0 && equals(field, "id") != 0){
            printf("Erro: A cláusula WHERE deve usar o campo 'codigo'!\n");
            return;
        }

        cursor = skipWhitespace(cursor);

        if(*cursor != '='){
            printf("Erro: Sintaxe inválida. Esperado '='!\n");
            return;
        }
        cursor++;

        char id[20];
        int typeID = readToken(&cursor, id);
        if(typeID == 2) { 
            printf("Erro: 'codigo' deve ser numérico.\n"); return; 
        }
        int targetCode = atoi(id);

        petNode *found = searchPet(listPet, targetCode);
        if(found){
            showPet(found); // Sua função showPerson existente
        } else {
            printf("Pet com código %d não encontrado.\n", targetCode);
        }
        return;
    } else {
        if(*cursor == '\0' || *cursor == ';'){
            if(listPet->counter == 0){
                printf("Tabela PET vazia.\n");
                return;
            }
        
            printf("\n--- Listando todos os PETs ---\n");

            petNode *node = listPet->head;
            while(node){
                showPet(node); 
                node = node->next;
            }
        } else {
            printf("Erro: Sintaxe incorreta no SELECT.\n");
        }
    }
}