#include <stdio.h>
#include <stdio.h>
#include <sys/types.h>
//#include <sys/wait.h>
#include <unistd.h>

#define BUFSIZE 128

int main(int argc, char* argv[]) {

    int fd;
    char buf[BUFSIZE];
    ssize_t ret;

    char shellstart[BUFSIZE] = "$ ./enseash\n";
    char Outputestline1[BUFSIZE] = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n";
    char endshell[BUFSIZE] = "enseash %\n";

    write(STDOUT_FILENO, shellstart, BUFSIZE);
    write(STDOUT_FILENO, Outputestline1, BUFSIZE);
    write(STDOUT_FILENO, endshell, BUFSIZE);
}

