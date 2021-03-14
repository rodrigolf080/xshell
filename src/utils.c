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

/*
 * Greeting shell during startup as in clear screen and print welcome text
 *
 * @param :: void
 */
void init_shell() 
{ 
    clear(); 
    printf("\n\n\n\n\t******************"
        "************************"); 
    printf("\n\n\n\t            ***** XSH *****       "); 
    printf("\n\n\t  ***** Open Source Unix Shell *****"); 
    printf("\n\n\n\n\t*******************"
        "***********************"); 
    printf("\n"); 
    sleep(3); 
    clear(); 
} 

/*
 * Take input string
 * Remove newline at the end
 * Apply a null byte terminator
 * @param :: str to be format
 */
void stringNullTerminate(char *str) {
  if (str == NULL)
    return;
  int length = strlen(str);
  if (str[length-1] == '\n')
    str[length-1]  = '\0';
}

/*
 * Take user input, read it from tty, save input and add to cmd history
 * Return the initial str with the user input in it
 * @param :: empty *str
 */
int takeInput(char *str) 
{ 
    char* buf; 
  
    buf = readline("$ "); 
    if (strlen(buf) != 0) { 
        add_history(buf); 
        strcpy(str, buf); 
        return 0; 
    } else { 
        return 1; 
    } 
} 

/*
 * Get env var $USER (name)
 * Get CWD (current working directory path)
 * Print typical shell line with user@path structure
 *
 * @param :: void
 * @return :: *str with user input 
 */
void printDir() 
{ 
    char cwd[1024]; 
    char *username = getenv("USER");    
    getcwd(cwd, sizeof(cwd)); 
    printf("%s", username);
    printf("@");
    printf("%s", cwd); 
} 

/*
 * Tree directory viewer - prints all files and sub-directories of a given 
 * directory in tree structure.
 * 
 * @param basePath :: base path to traverse directory
 * @param root :: integer representing indention for current directory (depth)
 */
void tree(char *basePath, const int root)
{
    int i;
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    // Unable to open directory
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL) // list recursively
    {
        // ignore previous and current directory repetition
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            // manage depth on tree branches
            for (i=0; i<root; i++) 
            {
                if (i%2 == 0 || i == 0)
                    printf("--::");
                else
                    printf("");

            }
            // list contents of current directory as "root" path
            printf("--::%s\n", dp->d_name);

            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            tree(path, root + 2);
        }
    }

    closedir(dir);
}


/*
 * Forks parent process and executes command on child then kills child process
 * Use C System lib execvp for system command execution
 * 
 * @param parsed :: first parsed command
 */
void execArgs(char **parsed) 
{ 
    // Forking a child 
    pid_t pid = fork();  
    
    // Parent returns >1
    // Child success returns 0
    // Forking failure returns <1

    if (pid == -1) { 
        printf("\nFailed forking child"); 
        return; 
    } else if (pid == 0) { 
        if (execvp(parsed[0], parsed) < 0) { 
            printf("\nCould not execute command error: 0\n"); 
        } 
        exit(0); 
    } else { 
        // Waiting for child to terminate 
        wait(NULL);  
        return; 
    } 
} 


/*
 * Forks parent process and executes first command
 * Pass output from first command to input on second command
 * Execute second command on another child process
 * Use C System lib execvp for system command execution
 *
 * @param parsed :: first parsed command
 * @param parsed :: second parsed command
 */
void execArgsPiped(char **parsed, char **parsedpipe) 
{ 
    // pipes :: 0 is read end, 1 is write end 
    int pipefd[2];  
    pid_t p1, p2; 
  
  	// return -1 on failure
    if (pipe(pipefd) < 0) { 
        printf("\nPipe could not be initialized\n"); 
        return; 
    } 
    // return child :: 0 success
    // return parent :: 1 success -1 failure
    p1 = fork(); 
    if (p1 < 0) { 
        printf("\nCould not fork\n"); 
        return; 
    } 
  
    // 0 executes on child and >1 on parent
    if (p1 == 0) { 
        // Child 1 executing.. 
        // It only needs to write at the write end 
        close(pipefd[0]); 
        dup2(pipefd[1], STDOUT_FILENO); 
        close(pipefd[1]); 
  
        if (execvp(parsed[0], parsed) < 0) { 
            printf("\nCould not execute command error: 1\n"); 
            exit(0); 
        } 
    } else { 
        // Parent executing 
        p2 = fork(); 
  
        if (p2 < 0) { 
            printf("\nCould not fork\n"); 
            return; 
        } 
  
        // Child 2 executing.. 
        // It only needs to read at the read end 
        if (p2 == 0) { 
            close(pipefd[1]); 
            dup2(pipefd[0], STDIN_FILENO); 
            close(pipefd[0]); 
            if (execvp(parsedpipe[0], parsedpipe) < 0) { 
                printf("\nCould not execute command error: 2..\n"); 
                exit(0); 
            } 
        } else { 
            // parent executing, waiting for last child
            wait(NULL); 
        } 
    } 
} 


/*
 * Display help section with available commands and features
 *
 * @param :: void
 */
void openHelp() 
{ 
    puts("\n***WELCOME TO XSH HELP***"
        "\nList of Commands supported:"
        "\n>cls - clear screen"
        "\n>cd - change directory"
        "\n>quit - quit shell"
        "\n>dir - list files recursively starting"
        " on the current directory in a tree-liek structure"
        "\n>all other general system commands available in UNIX shell"
        "\n>pipe handling"
        "\n>improper space handling"
        "\n>script execution\n"); 

    return; 
} 

/*
 * Execute builtin commands
 *
 * @param :: void
 */
int ownCmdHandler(char **parsed) 
{ 
    // Build an array of available commands to use
    // When user input matches this commands cast the according switch case
    // With command functionality

    int NoOfOwnCmds = 6, i, switchOwnArg = 0; 
    char* ListOfOwnCmds[NoOfOwnCmds]; 
    char* username;
  
    ListOfOwnCmds[0] = "quit"; 
    ListOfOwnCmds[1] = "cd"; 
    ListOfOwnCmds[2] = "help"; 
    ListOfOwnCmds[3] = "hello";
    ListOfOwnCmds[4] = "cls"; 
    ListOfOwnCmds[5] = "dir";
  
    for (i = 0; i < NoOfOwnCmds; i++) { 
        // strcmp returns 0 in case of match
        // we make the flag have the same value of the according switch statemnt
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) { 
            switchOwnArg = i + 1; 
            break; 
        } 
    } 
  
    switch (switchOwnArg) { 
    case 1: 
        printf("\nGoodbye!\n");
        sleep(1);
        clear(); 
        exit(0); 
    case 2: 
    	if (chdir(parsed[1]) == 0) {
    	} else {
    		printf("\nPath not found.\n");
    	}
        return 1; 
    case 3: 
        openHelp(); 
        return 1; 
    case 4: 
        username = getenv("USER"); 
        printf("\nHello %s!"
            "\nUse help to know more..\n", 
            username); 
        return 1;
    case 5:
        clear();
        return 1;
    case 6:
    	tree(".", 0);
        return 1;
    default: 
        break; 
    } 
  
    return 0; 
} 

/*
 * Look for pipe in a string
 * If found split the commands and remove the pipe
 *
 * @param str :: command string before pipe
 * @param strpiped :: command array after pipe is found
 */
int parsePipe(char *str, char **strpiped) 
{ 
    int i; 
    for (i = 0; i < 2; i++) { 
        strpiped[i] = strsep(&str, "|"); 
        if (strpiped[i] == NULL) 
            break; 
    } 
  
    if (strpiped[1] == NULL) 
        return 0; // returns zero if no pipe is found. 
    else { 
        return 1; 
    } 
} 

/*
 * Look for pipe in a string
 * If found split the commands and remove the pipe
 *
 * @param str :: command string before pipe
 * @param strpiped :: command array after pipe is found
 */
int parseIORedirectors(char *str, char **strRedirected) 
{ 
    return 0;
} 


/*
 * Look for spaces in input string
 * Split commands in an array
 * Don't store spaces in this array
 *
 * @param str :: string of commands to parse
 * @param parsed :: array of commands
 */
void parseSpace(char* str, char** parsed) 
{ 
    int i; 
  
    // remove space and go back on the array to manage empty space
    for (i = 0; i < MAXLIST; i++) { 
        parsed[i] = strsep(&str, " "); 
  
        if (parsed[i] == NULL) 
            break; 
        if (strlen(parsed[i]) == 0) 
            i--; 
    } 
} 

/*
 * Handle Piping and IO redirection
 * Split commands in an array
 * Clear spaces from this array
 *
 * @param str :: string of commands to parse
 * @param parsed :: array of commands
 * @param parsed :: array of commands
 */
int processString(char *str, char **parsed, char **parsedpipe, char **parsedArgsIORedirected) 
{ 
  
    char* strpiped[2];
    char* strRedirected[2];

    int piped = 0; 
    int redirected = 0;

  
    piped = parsePipe(str, strpiped);
    redirected = parseIORedirectors(str, strRedirected);

    if (piped) { 
        parseSpace(strpiped[0], parsed); 
        parseSpace(strpiped[1], parsedpipe); 
  
    } else { 
    	if (redirected) {
    		parseSpace(strRedirected[0], parsed); 
        	parseSpace(strRedirected[1], parsedArgsIORedirected);
    	} else {
    		parseSpace(str, parsed); 
    	}
    } 
  
    if (ownCmdHandler(parsed))
        return 0; 
    else
    	// returns 1 on simple command and 2 on piped command
        return 1 + piped; 
} 

/*
 * Process string into an array of commands
 * Handle spaces, piping and IO redirection
 *
 * @param inputString :: command string of user input
 */
void execParsedArgs(char *inputString)
{
    char *parsedArgs[MAXLIST]; 
    char *parsedArgsPiped[MAXLIST];
    char *parsedArgsIORedirected[MAXLIST];

    int execFlag = 0;
    // process 
    execFlag = processString(inputString, 
    parsedArgs, parsedArgsPiped, parsedArgsIORedirected); 
    // execflag returns zero if there is no command 
    // or it is a builtin command, 
    // 1 if it is a simple command 
    // 2 if it is including a pipe. 

    // execute 
    if (execFlag == 1) {
        execArgs(parsedArgs); 
    }
    if (execFlag == 2) {
        execArgsPiped(parsedArgs, parsedArgsPiped);
    }
    if (execFlag == 3) {
        //execArgsPiped(parsedArgs, parsedArgsPiped);
        printf("I was redirected with a return value of 3!\n");
    }
    if (execFlag == 4) {
        //execArgsPiped(parsedArgs, parsedArgsPiped);
        printf("I was redirected with a return value of 3!\n");
    }

}

/*
 * Manage script execution
 * When program is called ./shell scriptfile
 * Execute command in order
 *
 * @param argv :: command string of user input
 */
void execScript(char **script, char *inputString)
{
    FILE *fp;

    if ((fp = fopen(*script, "r")) == NULL){
        printf("File >>> %s <<< not found.\n", *script);
        printf("Quitting...\n\n");
        exit(0);
    }
    else {
        // open file and parse commands
        printf("Executing from file...\n");
        // need to go over each line
        // store each line
        // execparsedargs of each line
        while (fgets(inputString, MAXCOM,fp) != NULL) {
            stringNullTerminate(inputString);
            execParsedArgs(inputString);
        }
        //fgets(inputString, MAXCOM, fp);
        //printf( "%s" , inputString);

        fclose(fp);
        exit(0);
    }   
}
