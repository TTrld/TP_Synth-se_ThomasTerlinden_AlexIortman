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
int status;

char Shellstart[BUFSIZE] = "$ ./enseash\n";
char Outputestline1[BUFSIZE] = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n";
char Endshell[BUFSIZE] = "enseash % ";
char command[BUFSIZE];


int main(int argc, char* argv[]) {
	write(STDOUT_FILENO, Shellstart, strlen(Shellstart));
    write(STDOUT_FILENO, Outputestline1, strlen(Outputestline1));
    write(STDOUT_FILENO,Endshell,strlen(Endshell));

    
    while(1){
		
		
		
		ret = read(STDIN_FILENO,command,BUFSIZE);
		if (ret < 0){
			perror("unknown command");
			exit(EXIT_FAILURE);
		}
		
		if (ret > 0 && command[ret - 1] == '\n') {
			command[ret - 1] = '\0';
        }
        
        command[ret]='\0';
        
		if (strcmp(command ,"exit")==0){
			write(STDOUT_FILENO,"\nBye Bye...\n",12);
			break;
		}
		
		pid_t pid = fork();
        if (pid < 0) {
            perror("Erreur fork");
            exit(EXIT_FAILURE);
        }
		

        if (pid == 0) {
            execlp(command, command, (char *)NULL);
            perror("Command error execution");
            exit(EXIT_FAILURE);
        } else {
            waitpid(pid, &status, 0);
            
            if (WIFEXITED(status)) {
				
                int exit_code = WEXITSTATUS(status);
                char exit_message[BUFSIZE];
                snprintf(exit_message, sizeof(exit_message), "enseash [exit:%d] %% ", exit_code);
                write(STDOUT_FILENO, exit_message, strlen(exit_message));
                
            } else if (WIFSIGNALED(status)) {
				
                int signal_num = WTERMSIG(status);
                char signal_message[BUFSIZE];
                snprintf(signal_message, sizeof(signal_message), "enseash [sign:%d] %% ", signal_num);
                write(STDOUT_FILENO, signal_message, strlen(signal_message));
            }
        }
				
	}
	return 0;
}
