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
#include "builtins.h"

int main(int argc, char **argv)
{
	//char cmdline[MAXBUF];  stores user input from commmand line
	char* cmdline = (char*)malloc(MAXBUF);
	int exit = 0;

	while(exit == 0){
		printf("dsh> ");
		fgets(cmdline, MAXBUF, stdin);
		for(int i = 0; i < strlen(cmdline); i++){
			if(cmdline[i] == '\n'){
				cmdline[i] = '\0';
			}
		} 
		char** terms = split(cmdline, " ");
		// int i = 0;
		// while (terms[i] != NULL) {
		// 	printf("%s\n", terms[i]);
		// 	i++;
		// }
		if(terms == NULL){
			printf("Can't take an empty string\n");
		}
		else if(strcmp(cmdline, "exit") == 0){
			exit = 1;
		}
		else if(strcmp(terms[0], "pwd") == 0){
			char* dname = malloc(MAXBUF);
			getcwd(dname, MAXBUF);
			printf("%s\n", dname);
			free(dname);
		}
		else if(strcmp(terms[0], "cd") == 0){
			if(terms[1] == NULL){
				char* home = getenv("HOME");
				chdir(home);
			}
			else{
				chdir(terms[1]);
			}
		}
		else if(terms[0][0] == '/'){
			if (access(terms[0], F_OK | X_OK) == 0) {
				if(0 != fork()){
					wait(NULL);
				}
				else{
					execv(terms[0], terms);
				}
			}
			else {
				printf("%s not found!\n", terms[0]);
			}
		}
		else{
			printf("meow\n");
		}
		free(terms);
	}
	free(cmdline);
	
	return 0;
}
