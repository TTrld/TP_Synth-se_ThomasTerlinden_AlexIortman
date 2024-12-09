# TP_Synthèse - Thomas Terlinden & Alex Iortoman
[LAB 1](EnseaSH.pdf)

Each question can be executed separatly. 

For question 1, we wrote a welcoming message by writing in the console (STDOUT_FILENO). 

For question 2, we need to fork the current process so that the child process could be replaced by the execution of the called programm. 

For question 3, we wrote "Bye bye..." when we close the shell.

For question 4, you should test the programm by calling the "fonction.c" in our shell. The output should be exit:111.

For question 5, we have to be careful to include the <time.h> library and to define the struct we need to save the current time. We then calculated the elapsed_time. A sleep call has been added to be sure it works properly. 

For question 6, we need to use the execvp function to pass a vector of arguments. So that's why we created an array of args. 

Question 7 is not finished but we decided to show you our progress. 
