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
#include <time.h>

void ChildState(int state,long exectime){ 
	char *var = malloc(PRINT_SIZE);
	if(WIFEXITED(state)){sprintf(var,"enseash [exit:%d|%ld ms] %%",WEXITSTATUS(state),exectime);}
	if(WIFSIGNALED(state)){sprintf(var,"enseash [sign:%d|%ld ms] %%",WTERMSIG(state),exectime);}
	write(STDOUT_FILENO,var,strlen(var));
	}

char** FormatArgs(char *args){ 
	char **argv=malloc(SpaceCount(args)+2); //the number of arguments is 1 + the number of spaces, then we add (char*) NULL
	int indexArgs = 0;
	int indexArgv = 0;
	int indexChar = 0;
	
	for(indexArgv=0;indexArgv<SpaceCount(args)+2;indexArgv++){
		argv[indexArgv]=malloc(strlen(args)); 
		}
		
	indexArgv=0;
	
	while(args[indexArgs]!='\0'){
		if(args[indexArgs]==' '){
			indexArgv++;
			indexArgs++;
			indexChar = 0;
			}
		else{
		argv[indexArgv][indexChar]=args[indexArgs];
		indexArgs++;
		indexChar++;
		}
		}
	argv[indexArgv+1]=(char*) NULL; 
	return argv;
	}
	
int SpaceCount(char *args){

	int index = 0;
	int count = 0;
	while(args[index]!='\0'){
		if(args[index]==' '){count+=1;}
		index++;
		}
	return count;
	}

int DetectChar(char **args,char * find){
	int index = 0;
	while(args[index]!=(char *)NULL){
		if(strcmp(args[index],find)==0){return index;}
		index++;
		}
	return -1;
	}

char** ResToFile(char **args){
	int desc_file;
	int index = DetectChar(args,">");
	char* file = args[index+1]; //the String after ">" is the file path
	
	args[index]=(char*) NULL;

	desc_file = open(file, O_RDWR);
	if(desc_file==-1){
		desc_file = open(file,O_RDWR | O_CREAT ,S_IRWXU | S_IRWXG | S_IRWXO); //if the requested file does not exist, we create it
	}
	dup2(desc_file,STDOUT_FILENO);
	return args;
}

void Display(char * text){
	write(STDOUT_FILENO,text,strlen(text));
	}

long FormatTime(struct timespec tpsPrev,struct timespec tpsNext){
	long ns,ms;
	ms = 1000*(tpsNext.tv_sec - tpsPrev.tv_sec);
	ns = (tpsNext.tv_nsec - tpsPrev.tv_nsec)/1000000;
	return(ms+ns);
	} 

int main(int argc, char **argv){
	

	char *prompt= malloc(COMMAND_SIZE);
	char *exitcmd = "exit";
	
	pid_t pid = 0; 
	ssize_t size = 0;
	
	struct timespec tps1;
	struct timespec tps2;

	int status;
	 
	Display(StartMsg);
	
	while(1){
	
	size = read(STDIN_FILENO,prompt,COMMAND_SIZE);
	
	if(size==0){
	Display("\nbye bye\n");
	exit(EXIT_FAILURE);
	}
	prompt[size-1] = '\0';		
	if(strcmp(prompt,exitcmd)==0){	
	Display("bye bye\n");
	exit(EXIT_FAILURE);
	}
	else{
		char **promptarg = malloc(SpaceCount(prompt)+2);
		pid=fork();
		if(pid==-1){exit(EXIT_FAILURE);}
		if (pid!=0){			 //Parent process
			
			clock_gettime(CLOCK_MONOTONIC,&tps1);
			wait(&status);
			clock_gettime(CLOCK_MONOTONIC,&tps2);
			
			ChildState(status,FormatTime(tps1,tps2));
			}
		else{					 //Child process
			promptarg = FormatArgs(prompt);			
			if(DetectChar(promptarg,">")!=-1){
				promptarg=ResToFile(promptarg);
				}
			if(DetectChar(promptarg,"<")!=-1){
				}
			execvp(promptarg[0],promptarg);
			Display("command not found \n");
			exit(EXIT_FAILURE);
			}
		}
	}
}



