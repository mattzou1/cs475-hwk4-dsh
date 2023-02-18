/*
 * main.c
 *
 *  Created on: Mar 17 2017
 *      Author: david
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include<sys/wait.h>
#include <string.h>
#include "dsh.h"

int main(int argc, char **argv)
{
	//stores command line input
	char* cmdline = (char*)malloc(MAXBUF);
	int exit = 0;

	while(exit == 0){
		//prompts user and gets input
		printf("dsh> ");
		fgets(cmdline, MAXBUF, stdin);
		//removes new line char
		for(int i = 0; i < strlen(cmdline); i++){
			if(cmdline[i] == '\n'){
				cmdline[i] = '\0';
			}
		} 
		//creates string array from user input seperated by spaces
		char** terms = split(cmdline, " ");
		//check if user inputs empty string
		if(terms == NULL){
			printf("Can't take an empty string\n");
		}
		//exit while loop if user types exit
		else if(strcmp(cmdline, "exit") == 0){
			exit = 1;
		}
		//gets current directory
		else if(strcmp(terms[0], "pwd") == 0){
			char* dname = malloc(MAXBUF);
			getcwd(dname, MAXBUF);
			printf("%s\n", dname);
			free(dname);
		}
		//changes directory
		else if(strcmp(terms[0], "cd") == 0){
			//check if user gives you direcotry or not
			if(terms[1] == NULL){
				chdir(getenv("HOME"));
			}
			else{
				chdir(terms[1]);
			}
		}
		//mode one
		else if(terms[0][0] == '/'){
			//check if file is valid
			if (access(terms[0], F_OK | X_OK) == 0) {
				//parent process
				if(0 != fork()){
					//only waits if last element is not &
					if(checkLastElement(terms) == 0){
						wait(NULL);
					}
				}
				//child process
				else{
					execv(terms[0], terms);
				}
			}
			else {
				printf("ERROR: %s not found!\n", terms[0]);
			}
		}
		//mode 2
		else{
			char* dname = malloc(MAXBUF);
			//gets paths from environmental variable and creates string array
			char** paths = split(getenv("PATH"), ":");
			int pathfound = 0;
			int i = 0;
			//get current directory and append user input onto it
			getcwd(dname, MAXBUF);
			strcat(dname, "/");
			strcat(dname, terms[0]);
			//loop through paths from enviornmental variable and checks if they are valid
			while(pathfound == 0 && paths[i] != NULL){
				//check if file is valid
				if (access(dname, F_OK | X_OK) == 0) {
					pathfound = 1;
					//parent process
					if(0 != fork()){
						//only waits if last element is not &
						if(checkLastElement(terms) == 0){
							wait(NULL);
						}
					}
					//child process
					else{
						terms[0] = dname;
						execv(dname, terms);
					}
				}
				//appends user input to path 
				strcpy(dname, paths[i]);
				strcat(dname, "/");
				strcat(dname, terms[0]);
				i++;
			}
			if(pathfound == 0){
				printf("ERROR: %s not found!\n", terms[0]);
			}
			free(dname);
			int j = 0;
			while (paths[j] != NULL) {
				free(paths[j]);
				paths[j] = NULL;
				j++;
			}
			free(paths);
		}
		int i = 0;
		while (terms[i] != NULL) {
			free(terms[i]);
			terms[i] = NULL;
			i++;
		}
		free(terms);
	}
	free(cmdline);
	
	return 0;
}
