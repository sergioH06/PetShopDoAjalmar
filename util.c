#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "util.h"

char* skipWhitespace(char *cursor){
    while (*cursor && isspace((unsigned char)*cursor)){
        cursor++;
    }
    return cursor;
}

char* checkKeyword(char *cursor, const char *keyword){
    while (*keyword) {
        if (!(*cursor) || tolower((unsigned char)*cursor) != tolower((unsigned char)*keyword)){
            return NULL;
        }
        cursor++;
        keyword++;
    }
    return cursor; 
}

int equals(const char *s1, const char *s2){
    while (*s1 && *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)){
            return (tolower((unsigned char)*s1) - tolower((unsigned char)*s2));
        }
        s1++;
        s2++;
    }
    return (tolower((unsigned char)*s1) - tolower((unsigned char)*s2));
}

int readToken(char **cursor, char *buffer){
    // 0 = Erro
    // 1 = Dado normal
    // 2 = String
    *cursor = skipWhitespace(*cursor);

    int i = 0;

    if(**cursor == '\0') return 0;

    if(**cursor == '\'' || **cursor == '\"'){
        char quote = **cursor;
        (*cursor)++;

        while(**cursor && **cursor != quote){
            buffer[i++] = **cursor;
            (*cursor)++;
        }

        if(**cursor == '\0'){
            printf("Erro: Feche as aspas!\n");
            return 0;
        }

        (*cursor)++;
        buffer[i] = '\0';
        return 2; //Com Aspas
    } else {
        while(**cursor && **cursor != ',' && **cursor != ')' && **cursor != ';' && **cursor != ' '){
            if (**cursor == '\"' || **cursor == '\'') {
                printf("Erro: Aspas inesperadas no meio do texto!\n");
                return 0;
            }

            buffer[i++] = **cursor;
            (*cursor)++;
        }
        while (i > 0 && isspace((unsigned char)buffer[i-1])) i--;
        buffer[i] = '\0';
        return 1; // Sem Aspas
    }
}

int validateBirthDate(const char *birthDate){
    if(strlen(birthDate) != 10){
        printf("Erro: Tamanho incorreto! Use 10 caracteres (DD/MM/AAAA).\n");
        return 0;
    }

    if(birthDate[2] != '/' || birthDate[5] != '/'){
        printf("Erro: Formato inválido! As barras '/' estão nos lugares errados.\n");
        printf("Formato correto: DD/MM/AAAA.\n");
        return 0;
    }

    char *cursor = (char*) birthDate;
    
    while(*cursor != '\0'){
        if(*cursor != '/' && (*cursor < '0' || *cursor > '9')){
            printf("Erro: Caractere '%c' não permitido! Use apenas números e '/'.\n", *cursor);
            return 0;
        }
        cursor++;
    }

    return 1;
}

int compareDates(const char *date1, const char *date2){
    int day1, month1, year1;
    int day2, month2, year2;

    sscanf(date1, "%d/%d/%d", &day1, &month1, &year1);
    sscanf(date2, "%d/%d/%d", &day2, &month2, &year2);

    if (year1 < year2) return -1;
    if (year1 > year2) return 1;

    if (month1 < month2) return -1;
    if (month1 > month2) return 1;

    if (day1 < day2) return -1;
    if (day1 > day2) return 1;

    return 0;
}