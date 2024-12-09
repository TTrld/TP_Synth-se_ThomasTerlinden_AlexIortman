#include <stdio.h>
#include "string.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define BUFSIZE 128

int fd;
char buf[BUFSIZE];
ssize_t ret;
int status;
struct timespec start, end;
long elapsed_time_ms;


char Shellstart[BUFSIZE] = "$ ./enseash\n";
char Outputestline1[BUFSIZE] = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n";
char Endshell[BUFSIZE] = "enseash % ";
char command[BUFSIZE];
char *args[BUFSIZE];//a table to collect the argument of the command

void findArguments(char *command, char *args[]) {//the functiun to create the list of arguments and finish it by NULL
    int i = 0;
    char *argument = strtok(command, " ");
    while (argument != NULL && i < BUFSIZE - 1) {
        args[i++] = argument;
        argument = strtok(NULL, " ");
    }
    args[i] = NULL;
}


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
			findArguments(command,args);//call of the functiun to create the list of arguments
            execvp(args[0],args);//we change from execlp to execvp to allow the use of multilple arguments that are in a form of a list that finish by the argument NULL
            perror("Command error execution");
            exit(EXIT_FAILURE);
        } else {
            clock_gettime(CLOCK_MONOTONIC, &start);
            waitpid(pid, &status, 0);
            clock_gettime(CLOCK_MONOTONIC, &end);
            
            elapsed_time_ms = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;

            
            if (WIFEXITED(status)) {
				
                int exit_code = WEXITSTATUS(status);
                char exit_message[BUFSIZE];
                snprintf(exit_message, sizeof(exit_message), "enseash [exit:%d|%ldms] %% ", exit_code, elapsed_time_ms);
                write(STDOUT_FILENO, exit_message, strlen(exit_message));
                
            } else if (WIFSIGNALED(status)) {
				
                int signal_num = WTERMSIG(status);
                char signal_message[BUFSIZE];
                snprintf(signal_message, sizeof(signal_message), "enseash [sign:%d|%ldms] %% ", signal_num, elapsed_time_ms);
                write(STDOUT_FILENO, signal_message, strlen(signal_message));
            }
        }
				
	}
	return 0;
}
