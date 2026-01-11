#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "person.h"
#include "type.h"
#include "pet.h"
#include "util.h"

void validateInsertPerson(personList *listPerson, char *cursor){
    printf("\n--- INSERT em PESSOA ---\n");
    char *next = NULL;
    int map[10];
    int counter = 0;

    int code = -1;
    char name[100] = "";
    char address[50] = "";
    char phone[20] = "";
    char birthDate[11] = "";

    cursor = skipWhitespace(cursor);

    if(*cursor == '('){
        cursor++;

        while(*cursor && *cursor != ')'){
            char field[50];
            if(!readToken(&cursor, field, sizeof(field))) return;

            if(counter >= 5){
                printf("Erro: Excesso de campos!\n");
                return;
            }

            if(equals(field, "codigo") == 0) map[counter] = 0;
            else if(equals(field, "nome") == 0) map[counter] = 1;
            else if(equals(field, "endereco") == 0) map[counter] = 2;
            else if(equals(field, "telefone") == 0) map[counter] = 3;
            else if(equals(field, "data_nascimento") == 0) map[counter] = 4;
            else {
                printf("Erro: Campo desconhecido '%s' para INSERT em PESSOA!\n", field);
                return;
            }
            counter++;

            cursor = skipWhitespace(cursor);

            if(*cursor == ','){
                cursor++;
            } else if(*cursor != ')'){
                printf("Erro: Sintaxe inválida. Esperada vírgula ',' separando os campos!\n");
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
                    case 0:
                        if(type == 2){
                            printf("Erro: 'codigo' deve ser um número!\n");
                            return;
                        }
                        code = atoi(value);
                        break;
                    case 1:
                        if(type != 2){
                             printf("Erro: 'nome' deve ser uma string!\n");
                            return;
                        }
                        strncpy(name, value, 99);
                        name[99] = '\0';
                        break;
                    case 2:
                        if(type != 2){
                            printf("Erro: 'endereco' deve ser uma string!\n");
                            return;
                        }
                        strncpy(address, value, 49);
                        address[49] = '\0';
                        break;
                    case 3:
                        if(type != 2){
                            printf("Erro: 'telefone' deve ser uma string!\n");
                            return;
                        }
                        strncpy(phone, value, 19);
                        phone[19] = '\0';
                        break;
                    case 4:
                        if(type != 2){
                            printf("Erro: 'data_nascimento' deve ser uma string!\n");
                            return;
                        }
                        strncpy(birthDate, value, 10);
                        birthDate[10] = '\0';
                        break;
                }

                cursor = skipWhitespace(cursor);

                if (*cursor == ',') {
                    cursor++;
                } else if (*cursor != ')') { 
                    printf("Erro: Sintaxe inválida. Esperada vírgula ',' separando os valores!\n");
                    return; 
                }
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
        printf("Erro: O campo 'codigo' é obrigatório para PESSOA!\n");
        error = 1;
    }
    if (strcmp(name, "") == 0) {
        printf("Erro: O campo 'nome' é obrigatório para PESSOA!\n");
        error = 1;
    }
    if (strcmp(birthDate, "") == 0) {
        printf("Erro: O campo 'nascimento' é obrigatório para PESSOA!\n");
        error = 1;
    }

    cursor = skipWhitespace(cursor);
    if (*cursor != '\0' && *cursor != ';') {
        printf("Erro: Sintaxe inválida! Caracteres inesperados no fim do comando.\n");
        return;
    }

    if(!error){
        if(registerPerson(listPerson, code, name, address, phone, birthDate)){
            printf("Comando INSERT processado para PESSOA com código %d.\n", code);
        }
    }
}

void validateUpdatePerson(personList *listPerson, char *cursor){
    printf("\n--- UPDATE em PESSOA ---\n");
    char *next = NULL;
    
    int targetCode = -1;
    char name[100] = "";
    char address[50] = "";
    char phone[20] = "";
    char birthDate[11] = "";

    cursor = skipWhitespace(cursor);

    if((next = checkKeyword(cursor, "set ")) == NULL){
        printf("Erro: Sintaxe incorreta! Cláusula SET inexistente!\n");
        return;
    }

    cursor = next;
    cursor = skipWhitespace(cursor);

    while(*cursor && checkKeyword(cursor, "where") == NULL){
        char field[50];
        char value[100];
        if(!readToken(&cursor, field, sizeof(field))) return;

        cursor = skipWhitespace(cursor);

        if(*cursor != '='){
            printf("Erro: Sintaxe incorreta. Esperado '='!\n");
            return;
        }
        cursor++;

        int type = readToken(&cursor, value, sizeof(value));
        if(type == 0) return;

        if(equals(field, "nome") == 0){
            if(type != 2) { printf("Erro: 'nome' deve ser uma string!\n"); return; }
            strncpy(name, value, 99);
            name[99] = '\0';
        } else if(equals(field, "endereco") == 0){
            if(type != 2) { printf("Erro: 'endereco' deve ser uma string!\n"); return; }
            strncpy(address, value, 49);
            address[49] = '\0';
        } else if(equals(field, "telefone") == 0){
            if(type != 2) { printf("Erro: 'telefone' deve ser uma string!\n"); return; }
            strncpy(phone, value, 19);
            phone[19] = '\0';
        } else if(equals(field, "data_nascimento") == 0){
            if(type != 2) { printf("Erro: 'data_nascimento' deve ser uma string!\n"); return; }
            strncpy(birthDate, value, 10);
            birthDate[10] = '\0';
        } else {
            printf("Erro: Campo desconhecido '%s' para UPDATE em PESSOA!\n", field);
            return;
        }

        cursor = skipWhitespace(cursor);

        if (*cursor == '\0' || checkKeyword(cursor, "where") != NULL) {
            break; 
        } if (*cursor == ',') {
            cursor++;
        } else {
            printf("Erro: Esperada vírgula ',' separando os campos!\n");
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
        printf("Erro: Sintaxe incorreta. Esperado '='!\n");
        return;
    }
    cursor++;

    char id[20];
    int typeID = readToken(&cursor, id, sizeof(id));
    if(typeID == 2) { 
        printf("Erro: 'codigo' deve ser um número!\n"); return; 
    }
    targetCode = atoi(id);

    cursor = skipWhitespace(cursor);
    if (*cursor != '\0' && *cursor != ';') {
        printf("Erro: Sintaxe inválida! Caracteres inesperados no fim do comando.\n");
        return;
    }

    if(updatePerson(listPerson, targetCode, name, address, phone, birthDate)){
        printf("Comando UPDATE processado para PESSOA com código %d.\n", targetCode);
    }
}

void validateDeletePerson(personList *listPerson, petList *listPet, char *cursor){
    printf("\n--- DELETE em PESSOA ---\n");
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
        printf("Erro: 'codigo' deve ser um número!\n"); return; 
    }
    targetCode = atoi(id);

    cursor = skipWhitespace(cursor);
    if (*cursor != '\0' && *cursor != ';') {
        printf("Erro: Sintaxe inválida! Caracteres inesperados no fim do comando.\n");
        return;
    }

    deletePetByPerson(listPet, targetCode);
    if(deletePerson(listPerson, targetCode)){
        printf("Comando DELETE processado para PESSOA com código %d (e seus PETs).\n", targetCode);
    }
}

void validateSelectPerson(personList *listPerson, char *cursor){
    printf("\n--- SELECT em PESSOA ---\n");
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
            else if(equals(field, "nome") == 0) rule = 2;
            else if(equals(field, "endereco") == 0) rule = 3;
            else if(equals(field, "telefone") == 0) rule = 4;
            else if(equals(field, "data_nascimento") == 0) rule = 5;
            else {
                printf("Erro: Campo %s desconhecido para SELECT em PESSOA.\n", field);
                return;
            }

            personTreeNode *root = NULL;
            personNode *current = listPerson->head;

            while (current != NULL) {
                root = insertPersonInTree(root, current->element, rule);
                current = current->next;
            }

            cursor = skipWhitespace(cursor);
            if (*cursor != '\0' && *cursor != ';') {
                printf("Erro: Sintaxe inválida! Caracteres inesperados no fim do comando.\n");
                return;
            }

            if(root == NULL) {
                printf("Nenhum registro encontrado.\n");
            } else {
                showPersonTreeInOrder(root);
            }

            freePersonTree(root);
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
            printf("Erro: 'codigo' deve ser numérico!\n"); return; 
        }
        int targetCode = atoi(id);

        cursor = skipWhitespace(cursor);
        if (*cursor != '\0' && *cursor != ';') {
            printf("Erro: Sintaxe inválida! Caracteres inesperados no fim do comando.\n");
            return;
        }

        personNode *found = searchPerson(listPerson, targetCode);
        if(found){
            showPerson(found); 
        } else {
            printf("Erro: PESSOA com código %d não encontrada.\n", targetCode);
        }
        return;
    } else {
        if(*cursor == '\0' || *cursor == ';'){
            if(listPerson->counter == 0){
                printf("Tabela PESSOA vazia.\n");
                return;
            }
        
            printf("\n--- Listando todas as PESSOAs ---\n");
            personNode *node = listPerson->head;
            while(node){
                showPerson(node); 
                node = node->next;
            }
        } else {
            printf("Erro: Sintaxe incorreta no SELECT.\n");
        }
    }
}