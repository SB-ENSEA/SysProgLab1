#include "enseash.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <fcntl.h>
#include <string.h>

void ChildState(int state){
	char *var = malloc(PRINT_SIZE);
	if(WIFEXITED(state)){sprintf(var,"enseash [exit:%d] %%",WEXITSTATUS(state));}
	if(WIFSIGNALED(state)){sprintf(var,"enseash [sign:%d] %%",WTERMSIG(state));}
	//strcat(var," %");
	write(STDOUT_FILENO,var,strlen(var));
	}


int main(int argc, char **argv){
	
	char *print= malloc(PRINT_SIZE);
	char *prompt= malloc(COMMAND_SIZE);
	char *exitcmd = "exit";
	
	pid_t pid = 0;
	ssize_t size = 0;
	int status;
	 
	print = "Welcome to ENSEA Tiny Shell \n Pour quitter, tapez 'exit'. \n";  //variable pour l'affichage
	write(STDOUT_FILENO,print,strlen(print));
	
	print = "enseash %";
	write(STDOUT_FILENO,print,strlen(print));
	
	while(1){
	
	size = read(STDIN_FILENO,prompt,COMMAND_SIZE);
	
	if(size==0){
	print="bye bye\n";
	write(STDOUT_FILENO,print,strlen(print));
	exit(1);
	}
	
	prompt[size-1] = '\0';
	
	if(strcmp(prompt,exitcmd)==0){	
	print="bye bye\n";
	write(STDOUT_FILENO,print,strlen(print));
	exit(1);
	}
	else{
		pid=fork();
		if (pid!=0){ //exec pour le père
			wait(&status);
			ChildState(status);
			}
		else{  //exec pour le fils
			execlp(prompt,prompt, (char*) NULL);
			}
		}
	}
}





