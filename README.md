# myshell
Command line interactive shell build in C

- Once executed, myshell displays a command line interface to the terminal. If executed with a bathfile, myshell should execute each line of the file just like if it received input from the CLI.

- Executes buildin commands as well as C System commands

- User input is expected in the form:
	command [args]
	command - is the program name to be executed by the shell
	args - is the array of arguments run with the program, some commands may not need args, like 'help' and 'quit'.

'help' - prints list of builin commands
'quit' - exit myshell
'dir' - list contents of current working directory
'cls' - clear screen
'copy' - duplicate first argument file name and place it in path/newName
'md' - create new directory
'rd' - remove empty directory
'print' - echo arguments to stdout

Resources :
- https://brennan.io/2015/01/16/write-a-shell-in-c/ 
- https://www.cs.cornell.edu/courses/cs414/2004su/homework/shell/shell.html 
- https://www.gnu.org/software/bash/manual/bash.pdf

Research:
- Programs are executed by forking the shell processed and executing the program on the new child process, which is killed at end of execution;
- Processes are kept running in a child process until terminated, while myshell keeps running as a parent process;
- Background processes run on the child node and without displaying output to the parent process;
- I/O redirection is implemented by changing the stdin and stdout of the program executed;
- Memory allocation, rellocation and memory management in C using buffers;
- stdin, stdout and stderr manipulation;
- User input reading, String slicing and splitting in C using string getline and string tokens;
- How to execute System commands in C;
- Read and write to a CLI using C;

To do:
 - add I/O redirection (>>, <, >) - change stdin and stdout on child forking
 - background execution - allow child processes to run on the bg at the same time as user interacts with parent ndoe
 - add piping - pipe stdout of 1st program to stdin of second, execute them in different processes with fork
