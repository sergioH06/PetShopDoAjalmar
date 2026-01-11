#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "type.h"
#include "pet.h"
#include "util.h"

void validateInsertPetType(petTypeList *listType, char *cursor){
    printf("\n--- INSERT em TIPO DE PET ---\n");
    char *next = NULL;
    int map[5];
    int counter = 0;
    int code = -1;
    char description[50] =  "";

    cursor = skipWhitespace(cursor);

    if(*cursor == '('){
        cursor++;

        while(*cursor && *cursor != ')'){

            char field[50];
            if(!readToken(&cursor, field, sizeof(field))) return;

            if(counter >= 2){
                printf("Erro: Excesso de campos!\n");
                return;
            }
            
            if(equals(field, "codigo") == 0) map[counter] = 0;
            else if(equals(field, "descricao") == 0) map[counter] = 1;
            else {
                printf("Erro: Campo '%s' desconhecido para TIPO DE PET!\n", field);
                return;
            }
            counter++;

            cursor = skipWhitespace(cursor);

            if(*cursor == ','){
                cursor++;
            } else if(*cursor != ')'){
                printf("Erro: Sintaxe inválida. Esperada vírgula ','!\n");
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
                int type = readToken(&cursor, value, sizeof(value));
                if(type == 0) return;
                
                switch(map[i]){
                    case 0: code = atoi(value); break;
                    case 1: strncpy(description, value, 49); description[49] = '\0'; break;
                }
                cursor = skipWhitespace(cursor);
                if(*cursor == ',') cursor++;
            }
            cursor = skipWhitespace(cursor);

            if (*cursor != ')') {
                printf("Erro: Excesso de valores no VALUES! Esperado ')'.\n");
                return;
            }
            if (*cursor == ')') cursor++;
        }
    }
    
    int error = 0;
    if (code == -1) {
        printf("Erro: O campo 'codigo' é obrigatório para TIPO DE PET.\n");
        error = 1;
    }
    if(strcmp(description, "") == 0){
        printf("Erro: O campo 'descricao' é obrigatório para TIPO DE PET.\n");
        error = 1;
    }

    cursor = skipWhitespace(cursor);
    if (*cursor != '\0' && *cursor != ';') {
        printf("Erro: Sintaxe inválida! Caracteres inesperados no fim do comando.\n");
        return;
    }

    if(!error){
        if(registerPetType(listType, code, description)){
            printf("Comando INSERT processado para TIPO DE PET com código %d.\n", code);
        }
    }
}

void validateUpdatePetType(petTypeList *listType, char *cursor){
    printf("\n--- UPDATE em TIPO DE PET ---\n");
    char *next = NULL;
    
    int targetCode = -1;
    char description[50] = "";

    cursor = skipWhitespace(cursor);

    if((next = checkKeyword(cursor, "set ")) == NULL){
        printf("Erro: Sintaxe incorreta. Cláusula SET inexistente!\n");
        return;
    }
    cursor = next;
    cursor = skipWhitespace(cursor);

    while(*cursor && checkKeyword(cursor, "where") == NULL){
        char field[50];
        char value[70];
        if(!readToken(&cursor, field, sizeof(field))) return;

        cursor = skipWhitespace(cursor);

        if(*cursor != '='){
            printf("Erro: Sintaxe inválida. Esperado '='!\n");
            return;
        }
        cursor++;

        int type = readToken(&cursor, value, sizeof(value));
        if(type == 0) return;

        if(equals(field, "descricao") == 0){
            if(type != 2){
                printf("Erro: 'descricao' deve ser uma string!\n");
                return;
            }
            strncpy(description, value, 49);
            description[49] = '\0';
        } else {
            printf("Erro: Campo '%s' desconhecido para UPDATE em TIPO DE PET.\n", field);
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
    if(!readToken(&cursor, where, sizeof(where))) return;

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
    int typeID = readToken(&cursor, id, sizeof(id));
    if(typeID == 2) { 
        printf("Erro: 'codigo' deve ser numérico.\n"); return; 
    }
    targetCode = atoi(id);

    cursor = skipWhitespace(cursor);
    if (*cursor != '\0' && *cursor != ';') {
        printf("Erro: Sintaxe inválida! Caracteres inesperados no fim do comando.\n");
        return;
    }

    if(updatePetType(listType, targetCode, description)){
        printf("Comando UPDATE processado para TIPO DE PET com código %d.\n", targetCode);
    } 
}

void validateDeletePetType(petTypeList *listType, petList *listPet, char *cursor){
    printf("\n--- DELETE em TIPO DE PET ---\n");
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
    if(!readToken(&cursor, where, sizeof(where))) return;

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
    int typeID = readToken(&cursor, id, sizeof(id));
    if(typeID == 2) { 
        printf("Erro: 'codigo' deve ser numérico.\n"); return; 
    }
    targetCode = atoi(id);

    cursor = skipWhitespace(cursor);
    if (*cursor != '\0' && *cursor != ';') {
        printf("Erro: Sintaxe inválida! Caracteres inesperados no fim do comando.\n");
        return;
    }

    deletePetByType(listPet, targetCode);
    if(deletePetType(listType, targetCode)){
        printf("Comando DELETE processado para TIPO DE PET com código %d (e os PETs nessa categoria).\n", targetCode);
    }
}

void validateSelectPetType(petTypeList *listType, char *cursor){
    printf("\n--- SELECT em TIPO DE PET ---\n");

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
            if(!readToken(&cursor, field, sizeof(field))) return;

            if(equals(field, "codigo") == 0) rule = 1;
            else if(equals(field, "descricao") == 0) rule = 2;
            else {
                printf("Erro: Campo desconhecido '%s' para SELECT em TIPO DE PET!\n", field);
                return;
            }

            petTypeTreeNode *root = NULL;
            petTypeNode *current = listType->head;

            while (current != NULL) {
                root = insertPetTypeInTree(root, current->element, rule);
                current = current->next;
            }
            
            cursor = skipWhitespace(cursor);
            if (*cursor != '\0' && *cursor != ';') {
                printf("Erro: Sintaxe inválida! Caracteres inesperados no fim do comando.\n");
                return;
            }

            if (root == NULL) {
                printf("Nenhum registro encontrado.\n");
            } else {
                showPetTypeTreeInOrder(root);
            }

            freePetTypeTree(root);
            return;
        }
    } else if((next = checkKeyword(cursor, "where ")) != NULL){
        cursor = next;
        cursor = skipWhitespace(cursor);

        char field[50];
        if(!readToken(&cursor, field, sizeof(field))) return;

        if(equals(field, "codigo") != 0){
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
        int typeID = readToken(&cursor, id, sizeof(id));
        if(typeID == 2) { 
            printf("Erro: 'codigo' deve ser um número.\n"); return; 
        }
        int targetCode = atoi(id);

        cursor = skipWhitespace(cursor);
        if (*cursor != '\0' && *cursor != ';') {
            printf("Erro: Sintaxe inválida! Caracteres inesperados no fim do comando.\n");
            return;
        }
        
        petTypeNode *found = searchPetType(listType, targetCode);
        if(found){
            showPetType(found);
        } else {
            printf("TIPO DE PET com código %d não encontrada.\n", targetCode);
        }
        return;
    } else {
        if(*cursor == '\0' || *cursor == ';'){
            if(listType->counter == 0){
                printf("Tabela TIPO DE PET vazia.\n");
                return;
            }
        
            printf("\n--- Listando todos os TIPOs de PET ---\n");

            petTypeNode *node = listType->head;
            while(node){
                showPetType(node); 
                node = node->next;
            }
        } else {
            printf("Erro: Sintaxe inválida no SELECT.\n");
        }
    }
}