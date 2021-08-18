#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Shell behaviour
  1. Command is entered and if length is non-null, keep it in history.
  2. Parsing : Parsing is the breaking up of commands into individual words and strings
  3. Checking for special characters like pipes, etc is done
  4. Checking if built-in commands are asked for.
  5. If pipes are present, handling pipes.
  6. Executing system commands and libraries by forking a child and calling execvp.
  7. Printing current directory name and asking for next input.
 */
/*
  function declarations for builtin shell commands
 */
int myshell_cd(char **args);
int myshell_help();
int myshell_quit();
int myshell_cls();
int myshell_copy(char **args);
int myshell_md(char **args);
int myshell_rd(char **args);
int myshell_print(char **args);
int myshell_dir(char **args);

/*
  list of builtin commands
 */
char *builtin_str[] = {
    "cd",
    "help",
    "quit",
    "cls",
    "copy",
    "md",
    "rd",
    "dir",
    "print"
};
// array of function pointers arr[string]->int
int (*builtin_func[]) (char **) = {
    &myshell_cd,
    &myshell_help,
    &myshell_quit,
    &myshell_cls,
    &myshell_copy,
    &myshell_md,
    &myshell_rd,
    &myshell_dir,
    &myshell_print
};
// get number of builtin function 
int myshell_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  builtin function implementations
*/

//print builtins
int myshell_help()
{
  int i;
  printf("---- myshell -----\n");
  printf("command arg0 arg1 arg2 ...\n");
  printf("built-in commands:\n");

  for (i = 0; i < myshell_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  return 1;
}
// exit myshell
int myshell_quit()
{
  return 0;
}
// clear screen
int myshell_cls()
{
  printf("\e[1;1H\e[2J");
  return 1;
}

// fork child process to execute a program on
// execute system command
int myshell_launch(char **args)
{ 
  // child process should return 0 when forked or error
  // save pid to memory
  // run command name and array of args(execvp)
  // wait for program to finish executing
  // prompt for input again
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
      // child process
      // execute System command case commands is not builtin
      // if exec returns something, an error occured
      // handle and quit
      if (execvp(args[0], args) == -1) {
          perror("myshell");
      }
      exit(EXIT_FAILURE);
  } else if (pid < 0) {
      // print forking error
      perror("myshell");
  } else {
      // parent process wait for 
      // child execution to finish or be killed
      do {
        wpid = waitpid(pid, &status, WUNTRACED);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}

// execute built in commands
int myshell_execute(char **args)
{
    int i;

    if (args[0] == NULL) {
    	// check empty args
    	return 1;
    }
    // execute correct program by name with the array of args
    for (i = 0; i < myshell_num_builtins(); i++) {
	    if (strcmp(args[0], builtin_str[i]) == 0) {
	        return (*builtin_func[i])(args);
	    	}
		}

	return myshell_launch(args);
}

// alias
int myshell_copy(char **args)
{
	args[0] = "cp";
	myshell_launch(args);
	return 1;
}
int myshell_print(char **args)
{
	args[0] = "echo";
	myshell_launch(args);
	return 1;
}
int myshell_md(char **args)
{
	args[0] = "mkdir";
	myshell_launch(args);
	return 1;
}
int myshell_rd(char **args)
{
	args[0] = "rmdir";
	myshell_launch(args);
	return 1;
}
int myshell_dir(char **args)
{
  args[0] = "ls";
  myshell_launch(args);
  return 1;
}
// check first argument 
// change dir
// handle errors
int myshell_cd(char **args)
{
  if (args[1] == NULL){ 
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    printf("%s", cwd);
  } else {
    if (chdir(args[1]) != 0) {
      perror("myshell");
    }
  }
  return 1;
}

// read user input from the command line
// parse it when 'Enter' is pressed
// reserve 128 bytes in memory to buffer
// buff chars to existing String
#define MYSHELL_RL_BUFFERSIZE 1024
// read user input 
// alternative to getline() from stdio.h
char *myshell_read_line(void)
{ 
  int buffersize = MYSHELL_RL_BUFFERSIZE;
  // line position
  int pos = 0;
  // allocate requested memory and point buffer to it
  // accumulate characters in the buffer and parse the line
  // when the Enter key is pressed
  char *buffer = malloc(sizeof(char) * buffersize);
  int tmp;

  // check for allocation errors
  if (!buffer) {
      fprintf(stderr, "myshell: buffer allocation error\n");
      exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character and buff (as int)
    // while check EOF newline is false
    // \n (press Enter) parses the data line accumulated in the buffer
    tmp = getchar();
    if (tmp == EOF || tmp == '\n') {
      buffer[pos] = '\0'; // null
        return buffer;
    }   else {
          buffer[pos] = tmp;
    }
    pos++;

    // dinamically expand buffer
    if (pos >= buffersize) {
        buffersize += MYSHELL_RL_BUFFERSIZE;
        buffer = realloc(buffer, buffersize);
      // check for allocation errors
      if (!buffer) {
          fprintf(stderr, "myshell: buffer allocation error\n");
          exit(EXIT_FAILURE);
      }
    }
  }
}

// reserve 8 bytes for each tokem command
#define MYSHELL_TOKEN_BUFFERSIZE 64
// accept commands separated by a whitespace
// define whitespaces
#define MYSHELL_TOKEN_DELIMIT " \t\r\n\a"

char **myshell_split_line(char *line)
{
  // get each argument by splitting 
  // them into tokens delimited by whitespaces
    int buffersize = MYSHELL_TOKEN_BUFFERSIZE;
    int pos = 0;
    // allocate 8 bytes for each tokem command
    char **tokens = malloc(buffersize * sizeof(char*));
    char *token;
    // catch allocation error
    if (!tokens) {
      fprintf(stderr, "myshell: allocation error\n");
      exit(EXIT_FAILURE);
    }
    // create an array of token pointers separated by whitespaces
    token = strtok(line, MYSHELL_TOKEN_DELIMIT);
    // add all tokens to memory 
    while (token != NULL) {
      // store token in array buffer
      tokens[pos] = token;
      pos++;
      // dinamically expand buffer
      // case its size is bigger than the buffersize previous allocation
      if (pos >= buffersize) {
        buffersize += MYSHELL_TOKEN_BUFFERSIZE;
        tokens = realloc(tokens, buffersize * sizeof(char*));
          // catch allocation error
          if (!tokens) {
            fprintf(stderr, "myshell: allocation error\n");
            exit(EXIT_FAILURE);
          }
      }
      // no more tokens left to retreat
      token = strtok(NULL, MYSHELL_TOKEN_DELIMIT);
    }
    // null terminated arr
    tokens[pos] = NULL;
    return tokens;
}
void myshell_run(void)
{
  // user input from CLI
  char *line;
  // arguments for command
  char **args;
  // execute command with args and check status
  int status;

  // -- before checking the status
  // 1. print prompt
  // 2. read and parse user input
  // 3. execute command

  // keep running until determited by status
  do {
    // print prompt 
    // get current working directory
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    printf("%s> ", cwd);
    // get line of user input from console
    // buff line to memory when 'Enter; is pressed or Ctrl-D 
    line = myshell_read_line();
    // parse line with a buffer
    // split it into tokens(args of the command)
    args = myshell_split_line(line);
    // run shell with args and determine running status
    // 'exec' command withn args
    // by forking a child process and execute child with program
    // control parent child relatioship with status
    status = myshell_execute(args);

    // release buffers
    free(line);
    free(args);

  } while (status);

}
/*
 * Take input string
 * Remove newline at the end
 * Apply a null byte terminator
 * @param :: str to be format
 */
void stringNullTerminate(char *str)
{
    if (str == NULL)
        return;
    int length = strlen(str);
    if (str[length - 1] == '\n')
        str[length - 1] = '\0';
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
    // arguments for command
    char **args;

    if ((fp = fopen(*script, "r")) == NULL)
    {
        printf("File >>> %s <<< not found.\n", *script);
        printf("Quitting...\n\n");
        exit(0);
    }
    else
    {
        // open file and parse commands
        printf("Executing from file...\n");
        // need to go over each line
        // store each line
        // execparsedargs of each line
        int id = fork();
        if (id == -1)
        {
            printf("Problem forking...\n");
        }
        else if (id == 0)
        {
            while (fgets(inputString, MYSHELL_RL_BUFFERSIZE, fp) != NULL)
            {
                // parse line with a buffer
                // split it into tokens(args of the command)
                
                // myshell_execute(&inputString);
              stringNullTerminate(inputString);
              //printf("%s\n", inputString);
              args = myshell_split_line(inputString);
              myshell_launch(args);
            }
        }
        else
        {
            wait(NULL);
        }

        //fgets(inputString, MAXCOM, fp);
        //printf( "%s" , inputString);

        fclose(fp);
        exit(0);
    }
}

int main(int argc, char **argv)
{	
	// run with config files if they exist
  // for line in batchfile execute command:args
  // separate line by ;
  char inputString[MYSHELL_RL_BUFFERSIZE];
  // check if program was called with a script file to be opened and executed
  if (argv[1] != NULL) {
      execScript(&argv[1], inputString);
  }

  // missing piping
  // missing bg processing
  // missing io redirection
  
	// run child shell process
	myshell_run();

	// clean up and shutdown

	return (EXIT_SUCCESS);
}
