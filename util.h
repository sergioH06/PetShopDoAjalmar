#ifndef UTIL_H
#define UTIL_H

char* skipWhitespace(char *cursor);

char* checkKeyword(char *cursor, const char *keyword);

int equals(const char *s1, const char *s2);

int readToken(char **cursor, char *buffer);

int validateBirthDate(const char *birthDate);

int compareDates(const char *date1, const char *date2);

#endif 