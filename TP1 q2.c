#include <stdio.h>
#include "string.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZE 128

int fd;
char buf[BUFSIZE];
ssize_t ret;

char Shellstart[BUFSIZE] = "$ ./enseash\n";
char Outputestline1[BUFSIZE] = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n";
char Endshell[BUFSIZE] = "enseash %\n";
char Command[BUFSIZE];


int main(int argc, char* argv[]) {
	write(STDOUT_FILENO, Shellstart, strlen(Shellstart));
    write(STDOUT_FILENO, Outputestline1, strlen(Outputestline1));
    
    while(1){
		ret = read(STDIN_FILENO,Command,BUFSIZE);
		if (ret < 0){
			perror("unknown command");
			exit(EXIT_FAILURE);
		}
		
		if (ret > 0 && Command[ret - 1] == '\n') {
			Command[ret - 1] = '\0';
        }
        
        Command[ret]='\0';
        
		if (strcmp(Command ,"exit")==0){
			break;
		}
		
		pid_t pid = fork();
        if (pid < 0) {
            perror("Erreur fork");
            exit(EXIT_FAILURE);
        }
		

        if (pid == 0) {
            execlp(Command, Command, (char *)NULL);
            perror("Erreur d'exécution de la commande");
            exit(EXIT_FAILURE);
        } else {
            wait(NULL);
            write(STDOUT_FILENO,Endshell,strlen(Endshell));
        }
				
	}
	return 0;
}
