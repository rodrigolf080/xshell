#ifndef MACROS_H
#define MACROS_H

#define MAXCOM 1000 // max number of chars supported 
#define MAXLIST 100 // max number of commands supported 

// Clearing the shell using escape sequence
#define clear() printf("\033[H\033[J"); 

// Helper Functions
void init_shell();
int takeInput(char *str);
void printDir();
void tree(char *basePath, const int root);
void execArgs(char **parsed); 
void execArgsPiped(char **parsed, char **parsedpipe);
void openHelp();
int ownCmdHandler(char **parsed); 
int parsePipe(char *str, char **strpiped); 
void parseSpace(char* str, char** parsed);
int processString(char *str, char **parsed, char **parsedpipe); 
void execParsedArgs(char *inputString);
void execScript(char **script, char *inputString);


#endif