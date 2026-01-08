#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#include "person.h"
#include "type.h"
#include "pet.h"
#include "command.h"
#include "util.h"

int main(){ 
    setlocale(LC_ALL, "Portuguese");

    personList *listPerson = (personList*) malloc(sizeof(personList));
    initPersonList(listPerson);

    petTypeList *listType = (petTypeList*) malloc(sizeof(petTypeList));
    initPetTypeList(listType);

    petList *listPet = (petList*) malloc(sizeof(petList));
    initPetList(listPet);


    loadDataPerson(listPerson);
    loadDataPetType(listType);
    loadDataPet(listPet);


    commandQueue *queueCommand = (commandQueue*) malloc(sizeof(commandQueue));
    initCommandQueue(queueCommand);

    readScript(queueCommand, "script.txt");

    processCommands(queueCommand, listPerson, listType, listPet);

    char buffer[500];       
    char acumulator[5000]; 
    acumulator[0] = '\0';

    printf("\n---------------------------------------------\n");
    printf("   MODO INTERATIVO \n");
    printf("---------------------------------------------\n");

    while(1){
        if(strlen(acumulator) == 0) printf("\nSQL> ");
        else printf("   -> "); 
        
        if(fgets(buffer, sizeof(buffer), stdin) == NULL) break;

        if(*buffer == '\n' && strlen(acumulator) == 0){
            continue; 
        }

        if(strlen(buffer) > 0 && buffer[strlen(buffer) - 1] != '\n'){
            
            printf("Erro: Linha muito longa! O limite é %d caracteres.\n", (int) sizeof(buffer));
            printf("Comando descartado para evitar erros.\n");
            
            //substituição do fflush(stdin)
            int character;
            while((character = getchar()) != '\n' && character != EOF);
            
            acumulator[0] = '\0'; 
            continue; 
        }

        if(strlen(acumulator) + strlen(buffer) >= sizeof(acumulator) - 1){
            printf("Erro: Acumulador cheio! Comando total muito grande.\n");
            acumulator[0] = '\0';
            continue;
        }

        strcat(acumulator, buffer);
        
        char *cursor = acumulator;
        char *end;
        int finish = 0;

        while((end = strchr(cursor, ';')) != NULL){
            cursor = skipWhitespace(cursor);

            *end = '\0';
            
            if(checkKeyword(cursor, "exit") != NULL){
                finish = 1;
                break;
            }

            enqueueCommand(queueCommand, cursor);
            processCommands(queueCommand, listPerson, listType, listPet);

            cursor = end + 1;
        }

        if(finish) break;

        if(*cursor != '\0'){
            int i = 0;
            while(cursor[i] != '\0'){
                acumulator[i] = cursor[i];
                i++;
            }
            acumulator[i] = '\0';
        } else {
            acumulator[0] = '\0';
        }
    }

    printf("\n--- Salvando Dados ---\n");
    saveDataPerson(listPerson);
    saveDataPetType(listType);
    saveDataPet(listPet);

    freePersonList(listPerson);
    freePetTypeList(listType);
    freePetList(listPet);
    freeCommandQueue(queueCommand);

    free(listPerson);
    free(listType);
    free(listPet);
    free(queueCommand);

    return 0;
}