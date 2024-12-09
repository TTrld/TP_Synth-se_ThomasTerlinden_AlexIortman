#include <stdio.h>
#include "string.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

#define BUFSIZE 128

int fd;
char buf[BUFSIZE];
ssize_t ret;
int status;
struct timespec start, end;
long elapsed_time_ms;
int input_fd = -1, output_fd = -1;//Variables used to change the redirectory of the input and the output


char Shellstart[BUFSIZE] = "$ ./enseash\n";
char Outputestline1[BUFSIZE] = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n";
char Endshell[BUFSIZE] = "enseash % ";
char command[BUFSIZE];
char *args[BUFSIZE];

void findArguments(char *command, char *args[]) {
    int i = 0;
    char *argument = strtok(command, " ");
    while (argument != NULL && i < BUFSIZE - 1) {
        args[i++] = argument;
        argument = strtok(NULL, " ");
    }
    args[i] = NULL;
}


void findRedirect(int *input_fd, int *output_fd){
	char *input_file=NULL;
	char *output_file=NULL;
	
	char *output_redirect=strchr(*command,">");//look if there is a ">" symbole and then watch the directory of the output redirect
	while (output_redirect!=NULL){
		*output_redirect = '\0';//
        output_file = strtok(output_redirect +1, " ");
	}
	
	char *input_redirect=strchr(*command,">");//look if there is a "<" symbole and then watch the directory of the input redirect
	while (input_redirect!=NULL){
		*input_redirect = '\0';
		input_file = strtok(input_redirect +1, " ");
	}
	
	//we test if the files exist and the change the file descriptor
	
	if (output_file!=NULL){
        *output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (*output_fd < 0) {
			perror("Error regarding the output file");
			exit(EXIT_FAILURE);
        }
        dup2(output_fd, STDOUT_FILENO);
        close(input_fd);
    }
    
	if (input_file!=NULL){
        *output_fd = open(input_file,O_RDONLY);
        if (*input_fd < 0) {
			perror("Error regarding the input file");
			exit(EXIT_FAILURE);
        }
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
        if (*input_fd < 0) {
            perror("Error regarding the input file");
            exit(EXIT_FAILURE);
        }
    }
		
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
			findRedirect(&input_fd, &output_fd);//to change the redirectory
			findArguments(command,args);
            execvp(args[0],args);
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

