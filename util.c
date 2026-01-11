#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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

int readToken(char **cursor, char *buffer, int size){
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
            if(i < size - 1){
                buffer[i++] = **cursor;
            }
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

            if(i < size - 1){
                buffer[i++] = **cursor;
            }
            (*cursor)++;
        }
        while (i > 0 && isspace((unsigned char)buffer[i-1])) i--;
        buffer[i] = '\0';
        return 1; //Sem Aspas
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
    
    int day, month, year;
    sscanf(birthDate, "%d/%d/%d", &day, &month, &year);

    time_t date = time(NULL);
    struct tm *now = localtime(&date);

    int currentYear = now->tm_year + 1900;
    int currentMonth = now->tm_mon + 1;
    int currentDay = now->tm_mday;

    if (year > currentYear) {
        printf("Erro: Data inválida! Estamos em %d/%d/%d.\n", currentDay, currentMonth, currentYear);
        return 0;
    }

    if (month < 1 || month > 12) {
        printf("Erro: Mês inválido!\n");
        return 0;
    }

    if (year == currentYear && month > currentMonth) {
        printf("Erro: Data inválida! Estamos em %d/%d/%d.\n", currentDay, currentMonth, currentYear);
        return 0;
    }

    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if(month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))){
        daysInMonth[2] = 29;
    }

    if(day < 1 || day > daysInMonth[month]){
        printf("Erro: Dia inválido! O mês %02d/%d tem apenas %d dias.\n", month, year, daysInMonth[month]);
        return 0;
    }

    if (year == currentYear && month == currentMonth && day > currentDay) {
        printf("Erro: Data inválida! Estamos em %d/%d/%d.\n", currentDay, currentMonth, currentYear);
        return 0;
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