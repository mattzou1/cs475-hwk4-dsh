/*
 * dsh.c
 *
 *  Created on: Aug 2, 2013
 *      Author: chiu
 */
#include "dsh.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <errno.h>
#include <err.h>
#include <sys/stat.h>
#include <string.h>

// TODO: Your function definitions (declarations in dsh.h)
int is_empty(const char *s) {
  while (*s != '\0') {
    if (!isspace((unsigned char)*s)){
        return 0;
    }
    s++;
  }
  return 1;
}

char** split(char* str, char* delim){
    if(is_empty(str)){
        return NULL;
    }
    int numTokens = 1; 
    for(int i = 0; i < strlen(str); i++){
        if(str[i] == *delim){
            numTokens++;
        }
    }
    char **tokens = (char**)malloc((numTokens + 1) * sizeof(char*));
    for (int i = 0; i < numTokens + 1; i++) {
        tokens[i] = (char*) malloc(16 * sizeof(char));
    }
    /* get the first token */
    char *token = strtok(str, delim);
   
    int i = 0; 
    /* walk through other tokens */
    while( token != NULL ) {
        strcpy(tokens[i], token);
        token = strtok(NULL, delim);
        i++;
    }
    tokens[i] = NULL;
    return tokens; 
}