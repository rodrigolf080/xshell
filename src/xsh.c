#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include<readline/readline.h> 
#include<readline/history.h> 
#include<dirent.h> 

#define MAXCOM 1000 // max number of chars supported 
#define MAXLIST 100 // max number of commands supported 

// Clearing the shell using escape sequence
#define clear() printf("\033[H\033[J"); 


// Greeting shell during startup 
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


// Function to take input 
int takeInput(char* str) 
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

 // Function to print Current Directory. 
void printDir() 
{ 
    char cwd[1024]; 
    char* username = getenv("USER");    
    getcwd(cwd, sizeof(cwd)); 
    printf("%s", username);
    printf("@");
    printf("%s", cwd); 
} 

/**
 * Tree, prints all files and sub-directories of a given 
 * directory in tree structure.
 * 
 * @param basePath Base path to traverse directory
 * @param root     Integer representing indention for current directory
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

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            for (i=0; i<root; i++) 
            {
                if (i%2 == 0 || i == 0)
                    printf("--::");
                else
                    printf("");

            }

            printf("--::%s\n", dp->d_name);

            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            tree(path, root + 2);
        }
    }

    closedir(dir);
}

/**
 * batchExec executes every line a batch file just like an individual command
 * 
 * @param batchfile     File to read and parse commands from
 */
// void batchExec()
// {
//     // for each line in the batch file
//     // you have to read it, parse it and execute it as a command
//     // when end of file is reached quit the program
//     FILE *fp;

//     fp = fopen()

// }

void execArgs(char** parsed) 
{ 
    // Forking a child 
    pid_t pid = fork();  
  
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


// Function where the piped system commands is executed 
void execArgsPiped(char** parsed, char** parsedpipe) 
{ 
    // 0 is read end, 1 is write end 
    int pipefd[2];  
    pid_t p1, p2; 
  
    if (pipe(pipefd) < 0) { 
        printf("\nPipe could not be initialized\n"); 
        return; 
    } 
    p1 = fork(); 
    if (p1 < 0) { 
        printf("\nCould not fork\n"); 
        return; 
    } 
  
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
            // parent executing, waiting for two children 
            wait(NULL); 
            wait(NULL); 
        } 
    } 
} 

// Help command builtin 
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
        "\n>execute batchfiles one at a time\n"); 
  
    return; 
} 

// Function to execute builtin commands 
int ownCmdHandler(char** parsed) 
{ 
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
    		// ok
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

// function for finding pipe 
int parsePipe(char* str, char** strpiped) 
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

// function for parsing command words 
void parseSpace(char* str, char** parsed) 
{ 
    int i; 
  
    for (i = 0; i < MAXLIST; i++) { 
        parsed[i] = strsep(&str, " "); 
  
        if (parsed[i] == NULL) 
            break; 
        if (strlen(parsed[i]) == 0) 
            i--; 
    } 
} 
  
int processString(char* str, char** parsed, char** parsedpipe) 
{ 
  
    char* strpiped[2]; 
    int piped = 0; 
  
    piped = parsePipe(str, strpiped); 
  
    if (piped) { 
        parseSpace(strpiped[0], parsed); 
        parseSpace(strpiped[1], parsedpipe); 
  
    } else { 
  
        parseSpace(str, parsed); 
    } 
  
    if (ownCmdHandler(parsed)) 
        return 0; 
    else
        return 1 + piped; 
} 

void execParsedArgs(char *inputString)
{
    char *parsedArgs[MAXLIST]; 
    char *parsedArgsPiped[MAXLIST];
    int execFlag = 0;
    // process 
    execFlag = processString(inputString, 
    parsedArgs, parsedArgsPiped); 
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
}

void execScript(char **argv)
{
    FILE *fp;
    char inputString[MAXCOM];

    if ((fp = fopen(argv[1], "r")) == NULL){
        printf("File >>> %s <<< not found.\n", argv[1]);
        printf("Quitting...\n\n");
        exit(0);
    }
    else {
        // open file and parse commands
        printf("Executing from file...\n");
        while (fgets(inputString, MAXCOM,fp) != NULL) {
            // and execute command
            //execParsedArgs(inputString);
            printf( "%s" , inputString);
            printf("I am a new line!\n");
        }
        fclose(fp);
        exit(0);
    }   

}


int main(int argc, char **argv) 
{   
    char inputString[MAXCOM];
    // check if program was called with a script file to be opened and executed
    if (argv[1] != NULL) {
        execScript(argv);
    }

    // shell greeting
    init_shell();

    while (1) { 
        // print shell line 
        printDir(); 
        // take input 
        if (takeInput(inputString))     
        continue; 
        // handle commands
        execParsedArgs(inputString);

    } 
    return 0; 
} 

