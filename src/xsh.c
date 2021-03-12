#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <readline/readline.h> 
#include <readline/history.h> 
#include <dirent.h>
// macros
#include "macros.h"

int main(int argc, char **argv) 
{   
    char inputString[MAXCOM];

    // check if program was called with a script file to be opened and executed
    if (argv[1] != NULL) {
        execScript(&argv[1], inputString);
    }

    // shell greeting
    init_shell();

    // shell interaction
    while (1) { 
        // print shell line 
        printDir(); 
        // take input 
        if (takeInput(inputString))     
        continue; 
        // handle commands
        printf( "%s" , inputString);
        execParsedArgs(inputString);
    } 
    return 0; 
} 

