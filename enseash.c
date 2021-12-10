#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <fcntl.h>


int main(int argc, char **argv){
	
	char *print;
	print = "Welcome to ENSEA Tiny Shell \n Pour quitter, tapez 'exit'. \n";
	write(STDOUT_FILENO,print,59); 

}

