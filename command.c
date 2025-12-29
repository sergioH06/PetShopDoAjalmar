#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "person.h"
#include "type.h"
#include "pet.h"
#include "util.h"
#include "person_cmd.h"
#include "type_cmd.h"
#include "pet_cmd.h"

void initCommandQueue(commandQueue *q){
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

void enqueueCommand(commandQueue *q, char command[500]){
    commandNode *new = malloc(sizeof(commandNode));
    if(!new){
        printf("Erro: Alocação de memória falhou!\n");
        return;
    }

    strncpy(new->command, command, sizeof(new->command) - 1);
    new->command[sizeof(new->command) - 1] = '\0';
    new->next = NULL;

    if(!q->head){
        new->previous = NULL;
        q->head = new;
        q->tail = new;
    } else {
        new->previous = q->tail;
        q->tail->next = new;
        q->tail = new;
    }

    q->size++;
}

commandNode* dequeueCommand(commandQueue *q){
    commandNode *node = q->head;

    if(!q->head){
        return NULL;
    }

    if(node == q->head && node == q->tail){
        q->head = NULL;
        q->tail = NULL;
    } else {
        q->head->next->previous = NULL;
        q->head = q->head->next;
    }

    q->size--;
    return node;
}

void readScript(commandQueue *q, const char *filename){
    FILE *file = fopen(filename, "r");
    if(!file){
        printf("Erro: Arquivo '%s' não encontrado!\n", filename);
        return;
    }

    printf("\n--- LENDO ARQUIVO '%s' ---\n", filename);

    char buffer[500];
    int index = 0;
    int character;

    while((character = fgetc(file)) != EOF){
        if(index == 0 && (character == ' ' || character == '\n' || character == '\t')){
            continue;
        }

        if(character == ';'){
            buffer[index] = ';';
            index++;
            buffer[index] = '\0';

            printf("LIDO: [%s]\n", buffer);

            enqueueCommand(q, buffer);
            index = 0;
        } else {
            if(index < 499){
                if(character == '\n' || character == '\t'){
                    buffer[index] = ' ';
                } else {
                    buffer[index] = (char) character;
                }
                index++;
            }
        }
    }
    fclose(file);
    printf("Script carregado! %d comandos na fila.\n", q->size);
}

void freeCommandQueue(commandQueue *l){
    commandNode *node = l->head;

    while(node){
        commandNode *current = node;
        node = node->next;
        free(current);
    }

    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
}

void processCommands(commandQueue *q, personList *listPerson, petTypeList *listType, petList *listPet){

    while(q->head){
        commandNode *node = dequeueCommand(q);
        char *cursor = node->command;
        char *next = NULL;

        cursor = skipWhitespace(cursor);

        // --- INSERT ---
        if((next = checkKeyword(cursor, "insert ")) != NULL){
            cursor = next;
            cursor = skipWhitespace(cursor);
        
            if((next = checkKeyword(cursor, "into ")) != NULL){
                cursor = next;
                cursor = skipWhitespace(cursor);
            
                if((next = checkKeyword(cursor, "pessoa")) != NULL){
                    cursor = next;
                    cursor = skipWhitespace(cursor);

                    validateInsertPerson(listPerson, cursor);
                } else if((next = checkKeyword(cursor, "tipo_pet")) != NULL){
                    cursor = next;
                    cursor = skipWhitespace(cursor);

                    validateInsertPetType(listType, cursor);
                } else if((next = checkKeyword(cursor, "pet")) != NULL){
                    cursor = next;
                    cursor = skipWhitespace(cursor);

                    validateInsertPet(listPet, listPerson, listType, cursor);
                } else {
                    printf("Erro: Tabela desconhecida!\n");
                }
            } else {
                printf("Erro: Sintaxe inválida. Esperado 'into'!\n");
            }
        } 
        // --- UPDATE ---
        else if((next = checkKeyword(cursor, "update ")) != NULL){
            cursor = next;
            cursor = skipWhitespace(cursor);

            if((next = checkKeyword(cursor, "pessoa")) != NULL){
                cursor = next;
                cursor = skipWhitespace(cursor);

                validateUpdatePerson(listPerson, cursor);
            } else if((next = checkKeyword(cursor, "tipo_pet")) != NULL){
                cursor = next;
                cursor = skipWhitespace(cursor);

                validateUpdatePetType(listType, cursor);
            } else if((next = checkKeyword(cursor, "pet")) != NULL){
                cursor = next;
                cursor = skipWhitespace(cursor);

                validateUpdatePet(listPet, listPerson, listType, cursor);
            } else {
                printf("Erro: Tabela desconhecida!\n");
            }
        } 
        // --- DELETE ---
        else if((next = checkKeyword(cursor, "delete ")) != NULL){
            cursor = next;
            cursor = skipWhitespace(cursor);

            if((next = checkKeyword(cursor, "from ")) != NULL){
                cursor = next;
                cursor = skipWhitespace(cursor);

                if((next = checkKeyword(cursor, "pessoa")) != NULL){
                    cursor = next;
                    cursor = skipWhitespace(cursor);

                    validateDeletePerson(listPerson, listPet, cursor);
                } else if((next = checkKeyword(cursor, "tipo_pet")) != NULL){
                    cursor = next;
                    cursor = skipWhitespace(cursor);

                    validateDeletePetType(listType, listPet, cursor);
                } else if((next = checkKeyword(cursor, "pet")) != NULL){
                    cursor = next;
                    cursor = skipWhitespace(cursor);

                    validateDeletePet(listPet, cursor);
                } else {
                    printf("Erro: Tabela desconhecida!\n");
                }
            } else {
                printf("Erro: Sintaxe inválida. Esperado 'from'!\n");
            }
        } 
        // --- SELECT ---
        else if((next = checkKeyword(cursor, "select ")) != NULL){
            cursor = next;
            cursor = skipWhitespace(cursor);
            
            if((next = checkKeyword(cursor, "* ")) != NULL){ 
                cursor = next;
                cursor = skipWhitespace(cursor);
                
                if((next = checkKeyword(cursor, "from ")) != NULL){ 
                    cursor = next;
                    cursor = skipWhitespace(cursor);
                    
                    if((next = checkKeyword(cursor, "pessoa")) != NULL){
                        cursor = next;
                        cursor = skipWhitespace(cursor);

                        validateSelectPerson(listPerson, cursor);
                    } else if((next = checkKeyword(cursor, "tipo_pet")) != NULL){
                        cursor = next;
                        cursor = skipWhitespace(cursor);

                        validateSelectPetType(listType, cursor);
                    } else if((next = checkKeyword(cursor, "pet")) != NULL){
                        cursor = next;
                        cursor = skipWhitespace(cursor);

                        validateSelectPet(listPet, cursor);
                    } else {
                        printf("Erro: Tabela desconhecida!\n");
                    }
                } else {
                     printf("Erro: Sintaxe inválida. Esperado 'from' após '*'!\n");
                }
            } else {
                printf("Erro: Sintaxe inválida. Esperado '*'!\n");
            }
        } else {
            printf("Erro: Comando inválido ou não reconhecido!\n");
        }
        free(node);
    }
}
