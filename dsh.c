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

//checks to see if a string is empty
int is_empty(const char *s) {
  while (*s != '\0') {
    if (!isspace((unsigned char)*s)){
        return 0;
    }
    s++;
  }
  return 1;
}

//checks to see if last element is an & sign
int checkLastElement(char** terms){
    int i = 0;
    while(terms[i] != NULL){
        i++;
    }
    if(strcmp(terms[i - 1], "&") == 0){
        return 1;
    }
    return 0;
}

//splits string into array based off a delim
char** split(char* str, char* delim){
    if(is_empty(str)){
        return NULL;
    }
    //find length of array
    int numTokens = 1; 
    for(int i = 0; i < strlen(str); i++){
        if(str[i] == *delim){
            numTokens++;
        }
    }
    char **tokens = (char**)malloc((numTokens + 1) * sizeof(char*));
    //malloc all char*s inside tokens
    for (int i = 0; i < numTokens; i++) {
        tokens[i] = (char*) malloc(128 * sizeof(char));
    }

    // get the first token 
    char *token = strtok(str, delim);
   
    int i = 0; 
    // walk through other tokens 
    while( token != NULL ) {
        strcpy(tokens[i], token);
        token = strtok(NULL, delim);
        i++;
    }
    //NULL terminate last element
    tokens[i] = NULL;
    return tokens; 
}